# NR5g_RAR_PDU_Decoder
RAR PDU decoder for NR5g for 4 step RACH process

Objective:
>>>RAR PDU decoder can be used to decode any MAC RAR pdu in case of RAR exchange during 4 step rach process of NR.

Inputs:
>>>Input 1: HEX Dump of RAR PDU to be supplied via code. Code needs to be recompiled.
>>>Input 2: Preamble IDs reserved for SI request Acknowledgement need to be update in global array in code.

Enhancement Proposed #1:
>>>Hex dump and Reserved Preamble Ids could be provided via config file or as command line argument.

Why Enhancement #1 is not already done ?
>>>Because Coder is lazy.

Outputs:
Currently it outputs BI, RAPID, TA, UL_GRANT and TC-RNTI.

Enhancement Proposed #2:
>>>UL_GRANT(27 bits) field can be further decoded into ul_grant, mcs, hopping flag etc.

Why Enhancement #2 is not already done ?
>>>Because Coder is lazy.
