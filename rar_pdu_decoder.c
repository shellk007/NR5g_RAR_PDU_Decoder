#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>

#define MAX_PREAMBLE_ID 64

uint8_t g_si_configured_rapid[MAX_PREAMBLE_ID] = {64,64,64,64,64,64,64,64,
                                                  64,64,64,64,64,64,64,64,
                                                  64,64,64,64,64,64,64,64,
                                                  64,64,64,64,64,64,64,64,
                                                  64,64,64,64,64,64,64,64,
                                                  64,64,64,64,64,64,64,64,
                                                  64,64,64,64,64,64,64,64,
                                                  64,64,64,64,64,64,64,64,
                                                 };
bool rapid_only(uint8_t rapid)
{
    bool found = false;

    for (int idx = 0; idx < MAX_PREAMBLE_ID; idx++){
        if(rapid ==g_si_configured_rapid[idx]){
            found = true;
            break;
        }
    }
    return(found);
}

int main(void)
{
    uint8_t    rar_buf[] = {0x4f,0x00,0x01,0xf5,0x1c,0xff,0x00,0xf2,0x00};
    uint64_t   offset = 0;
    uint8_t    e_bit, t_bit, rapid, bi=0;
    uint16_t   timing_advance = 0;
    uint32_t   ul_grant = 0;
    uint16_t   tc_rnti = 0;
    uint32_t   index = 0;

    //rar_buf = (uint8_t*)malloc(sizeof(argv[1]));

    //memcpy(rar_buf, atoi(argv[1]));

    while(1){
        /* Find E and check if there are more MAC SUBPDUs*/
        e_bit = rar_buf[offset]&0x80;

        /* Find T bit and check the type: i.e. BI or RAPID*/
        t_bit = rar_buf[offset]&0x40;

        if(t_bit){
            /* This is RAPID subheader */
            rapid = rar_buf[offset]&0x3F;
        }
        else{
            /* This is BI subheader */
            bi = rar_buf[offset]&0x0F;
            printf("\n Backoff indicator is included; BI value is: %d", bi);
            if(e_bit){
                offset++;
                continue;
            }
            else
                break;
        }

        /* Check if RAPID has been configured for SI request acknowledgement
         * In that case it would be a RAPID only subheader */
        if(rapid_only(rapid)){
            printf("\nThis subheader is a RAPID ONLY subheader, No corresponding MAC RAR exists for RAPID: %d", rapid);
            if(e_bit){
                offset++;
                continue;
            }
            else
                break;
        }

        /* We reach this point only in case of rapid with MAC_RAR */
        offset++;

        for(index = 0; index <sizeof(timing_advance); index++ ){
        
            timing_advance = timing_advance <<8;
            timing_advance = timing_advance | rar_buf[offset + index];
        }
        timing_advance = timing_advance & (0x7FF8);
        offset+=1;

        for(index = 0; index <sizeof(ul_grant); index++ ){
        
            ul_grant = ul_grant <<8;
            ul_grant = ul_grant | rar_buf[offset + index];
        }
        ul_grant = ul_grant & (0x07FFFFFF); /* Not decoding UL grant further, but it can be enhanced to do so */
        offset+=4;

        for(index = 0; index <sizeof(tc_rnti); index++ ){
        
            tc_rnti = tc_rnti <<8;
            tc_rnti = tc_rnti | rar_buf[offset + index];
        }
        offset+=2;

        printf("\n===========================================================================");
        printf("\n MAC RAR decoded for RAPID: %d", rapid);
        printf("\n TIMING Advance: %d", timing_advance);
        printf("\n UL grant: %d", ul_grant);
        printf("\n TC-RNTI: %d", tc_rnti);
        printf("\n===========================================================================");

        timing_advance = 0;
        ul_grant = 0;
        tc_rnti = 0;

        if(e_bit)
            continue;
        else
            break;
    }
    return(EXIT_SUCCESS);
}
