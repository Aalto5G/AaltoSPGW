/*----------------------------------------------------------------------------*
 *                                                                            *
 *                              n w - g t p v 2 c                             *
 *    G P R S   T u n n e l i n g    P r o t o c o l   v 2 c    S t a c k     *
 *                                                                            *
 *                                                                            *
 * Copyright (c) 2010-2011 Amit Chawre                                        *
 * All rights reserved.                                                       *
 *                                                                            *
 * Redistribution and use in source and binary forms, with or without         *
 * modification, are permitted provided that the following conditions         *
 * are met:                                                                   *
 *                                                                            *
 * 1. Redistributions of source code must retain the above copyright          *
 *    notice, this list of conditions and the following disclaimer.           *
 * 2. Redistributions in binary form must reproduce the above copyright       *
 *    notice, this list of conditions and the following disclaimer in the     *
 *    documentation and/or other materials provided with the distribution.    *
 * 3. The name of the author may not be used to endorse or promote products   *
 *    derived from this software without specific prior written permission.   *
 *                                                                            *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR       *
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES  *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.    *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,           *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT   *
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY      *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT        *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF   *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          *
 *----------------------------------------------------------------------------*/

/**
 * @file NwGtpv2cIe.h
 * @brief This header file contains Information Element definitions for GTPv2c
 * as per 3GPP TS 29274-930.
*/

#ifndef __NW_GTPV2C_IE_H__
#define __NW_GTPV2C_IE_H__

/*--------------------------------------------------------------------------*
 *   G T P V 2 C    I E  T Y P E     M A C R O     D E F I N I T I O N S    *
 *--------------------------------------------------------------------------*/

#define NW_GTPV2C_IE_RESERVED                                           (0)
#define NW_GTPV2C_IE_IMSI                                               (1)
#define NW_GTPV2C_IE_CAUSE                                              (2)
#define NW_GTPV2C_IE_RECOVERY                                           (3)
#define NW_GTPV2C_IE_APN                                                (71)
#define NW_GTPV2C_IE_AMBR                                               (72)
#define NW_GTPV2C_IE_EBI                                                (73)
#define NW_GTPV2C_IE_IP_ADDRESS                                         (74)
#define NW_GTPV2C_IE_MEI                                                (75)
#define NW_GTPV2C_IE_MSISDN                                             (76)
#define NW_GTPV2C_IE_INDICATION                                         (77)
#define NW_GTPV2C_IE_PCO                                                (78)
#define NW_GTPV2C_IE_PAA                                                (79)
#define NW_GTPV2C_IE_BEARER_LEVEL_QOS                                   (80)
#define NW_GTPV2C_IE_FLOW_QOS                                           (81)
#define NW_GTPV2C_IE_RAT_TYPE                                           (82)
#define NW_GTPV2C_IE_SERVING_NETWORK                                    (83)
#define NW_GTPV2C_IE_BEARER_TFT                                         (84)
#define NW_GTPV2C_IE_TAD                                                (85)
#define NW_GTPV2C_IE_ULI                                                (86)
#define NW_GTPV2C_IE_FTEID                                              (87)
#define NW_GTPV2C_IE_TMSI                                               (88)
#define NW_GTPV2C_IE_GLOBAL_CN_ID                                       (89)
#define NW_GTPV2C_IE_S103PDF                                            (90)
#define NW_GTPV2C_IE_S1UDF                                              (91)
#define NW_GTPV2C_IE_DELAY_VALUE                                        (92)
#define NW_GTPV2C_IE_BEARER_CONTEXT                                     (93)
#define NW_GTPV2C_IE_CHARGING_ID                                        (94)
#define NW_GTPV2C_IE_CHARGING_CHARACTERISTICS                           (95)
#define NW_GTPV2C_IE_TRACE_INFORMATION                                  (96)
#define NW_GTPV2C_IE_BEARER_FLAGS                                       (97)
#define NW_GTPV2C_IE_PDN_TYPE                                           (99)
#define NW_GTPV2C_IE_PROCEDURE_TRANSACTION_ID                           (100)
#define NW_GTPV2C_IE_DRX_PARAMETER                                      (101)
#define NW_GTPV2C_IE_UE_NETWORK_CAPABILITY                              (102)
#define NW_GTPV2C_IE_PDN_CONNECTION                                     (109)
#define NW_GTPV2C_IE_PDU_NUMBERS                                        (110)
#define NW_GTPV2C_IE_PTMSI                                              (111)
#define NW_GTPV2C_IE_PTMSI_SIGNATURE                                    (112)
#define NW_GTPV2C_IE_HIP_COUNTER                                        (113)
#define NW_GTPV2C_IE_UE_TIME_ZONE                                       (114)
#define NW_GTPV2C_IE_TRACE_REFERENCE                                    (115)
#define NW_GTPV2C_IE_COMPLETE_REQUEST_MESSAGE                           (116)
#define NW_GTPV2C_IE_GUTI                                               (117)
#define NW_GTPV2C_IE_F_CONTAINER                                        (118)
#define NW_GTPV2C_IE_F_CAUSE                                            (119)
#define NW_GTPV2C_IE_SELECTED_PLMN_ID                                   (120)
#define NW_GTPV2C_IE_TARGET_IDENTIFICATION                              (121)
#define NW_GTPV2C_IE_PACKET_FLOW_ID                                     (123)
#define NW_GTPV2C_IE_RAB_CONTEXT                                        (124)
#define NW_GTPV2C_IE_SOURCE_RNC_PDCP_CONTEXT_INFO                       (125)
#define NW_GTPV2C_IE_UDP_SOURCE_PORT_NUMBER                             (126)
#define NW_GTPV2C_IE_APN_RESTRICTION                                    (127)
#define NW_GTPV2C_IE_SELECTION_MODE                                     (128)
#define NW_GTPV2C_IE_SOURCE_IDENTIFICATION                              (129)
#define NW_GTPV2C_IE_CHANGE_REPORTING_ACTION                            (131)
#define NW_GTPV2C_IE_FQ_CSID                                            (132)
#define NW_GTPV2C_IE_CHANNEL_NEEDED                                     (133)
#define NW_GTPV2C_IE_EMLPP_PRIORITY                                     (134)
#define NW_GTPV2C_IE_NODE_TYPE                                          (135)
#define NW_GTPV2C_IE_FQDN                                               (136)
#define NW_GTPV2C_IE_TI                                                 (137)
#define NW_GTPV2C_IE_MBMS_SESSION_DURATION                              (138)
#define NW_GTPV2C_IE_MBMS_SERIVCE_AREA                                  (139)
#define NW_GTPV2C_IE_MBMS_SESSION_IDENTIFIER                            (140)
#define NW_GTPV2C_IE_MBMS_FLOW_IDENTIFIER                               (141)
#define NW_GTPV2C_IE_MBMS_IP_MULTICAST_DISTRIBUTION                     (142)
#define NW_GTPV2C_IE_MBMS_IP_DISTRIBUTION_ACK                           (143)
#define NW_GTPV2C_IE_RFSP_INDEX                                         (144)
#define NW_GTPV2C_IE_UCI                                                (145)
#define NW_GTPV2C_IE_CSG_INFORMATION_REPORTING_ACTION                   (146)
#define NW_GTPV2C_IE_CSG_ID                                             (147)
#define NW_GTPV2C_IE_CSG_MEMBERSHIP_INDICATION                          (148)
#define NW_GTPV2C_IE_SERVICE_INDICATOR                                  (149)
#define NW_GTPV2C_IE_PRIVATE_EXTENSION                                  (255)
#define NW_GTPV2C_IE_TYPE_MAXIMUM                                       (256)


/*--------------------------------------------------------------------------*
 *   G T P V 2 C      C A U S E      V A L U E     D E F I N I T I O N S    *
 *--------------------------------------------------------------------------*/
/* Request / Initial message */
#define NW_GTPV2C_CAUSE_LOCAL_DETACH                                         (2)
#define NW_GTPV2C_CAUSE_COMPLETE_DETACH                                      (3)
#define NW_GTPV2C_CAUSE_RAT_CHANGED_FROM_3GPP_TO_NON_3GPP                    (4)
#define NW_GTPV2C_CAUSE_ISR_DEACTIVATION                                     (5)
#define NW_GTPV2C_CAUSE_ERROR_INDICATION_RECEIVED_FROM_RNC_ENODEB_S4_SGSN    (6)
#define NW_GTPV2C_CAUSE_IMSI_DETACH_ONLY                                     (7)
#define NW_GTPV2C_CAUSE_REACTIVATION_REQUESTED                               (8)
#define NW_GTPV2C_CAUSE_PDN_RECONNECTION_TO_THIS_APN_DISALLOWED              (9)
#define NW_GTPV2C_CAUSE_ACCESS_CHANGED_FROM_NON_3GPP_TO_3GPP                 (10)
#define NW_GTPV2C_CAUSE_PDN_CONNECTION_INACTIVITY_TIMER_EXPIRES              (11)
#define NW_GTPV2C_CAUSE_PGW_NOT_RESPONDING                                   (12)
#define NW_GTPV2C_CAUSE_NETWORK_FAILURE                                      (13)
#define NW_GTPV2C_CAUSE_QOS_PARAMETER_MISMATCH                               (14)
/*Acceptance in a Response / triggered message. */
#define NW_GTPV2C_CAUSE_REQUEST_ACCEPTED                                     (16)
#define NW_GTPV2C_CAUSE_REQUEST_ACCEPTED_PARTIALLY                           (17)
#define NW_GTPV2C_CAUSE_NEW_PDN_TYPE_DUE_TO_NETWORK_PREFERENCE               (18)
#define NW_GTPV2C_CAUSE_NEW_PDN_TYPE_DUE_TO_SINGLE_ADDRESS_BEARER_ONLY       (19)
/* Rejection in a Response / triggered message.*/
#define NW_GTPV2C_CAUSE_CONTEXT_NOT_FOUND                                    (64)
#define NW_GTPV2C_CAUSE_INVALID_MESSAGE_FORMAT                               (65)
#define NW_GTPV2C_CAUSE_VERSION_NOT_SUPPORTED_BY_NEXT_PEER                   (66)
#define NW_GTPV2C_CAUSE_INVALID_LENGTH                                       (67)
#define NW_GTPV2C_CAUSE_SERVICE_NOT_SUPPORTED                                (68)
#define NW_GTPV2C_CAUSE_MANDATORY_IE_INCORRECT                               (69)
#define NW_GTPV2C_CAUSE_MANDATORY_IE_MISSING                                 (70)
#define NW_GTPV2C_CAUSE_SYSTEM_FAILURE                                       (72)
#define NW_GTPV2C_CAUSE_NO_RESOURCES_AVAILABLE                               (73)
#define NW_GTPV2C_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION                  (74)
#define NW_GTPV2C_CAUSE_SYNTACTIC_ERROR_IN_THE_TFT_OPERATION                 (75)
#define NW_GTPV2C_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER_S                   (76)
#define NW_GTPV2C_CAUSE_SYNTACTIC_ERRORS_IN_PACKET_FILTER_S                  (77)
#define NW_GTPV2C_CAUSE_MISSING_OR_UNKNOWN_APN                               (78)
#define NW_GTPV2C_CAUSE_GRE_KEY_NOT_FOUND                                    (80)
#define NW_GTPV2C_CAUSE_RELOCATION_FAILURE                                   (81)
#define NW_GTPV2C_CAUSE_DENIED_IN_RAT                                        (82)
#define NW_GTPV2C_CAUSE_PREFERRED_PDN_TYPE_NOT_SUPPORTED                     (83)
#define NW_GTPV2C_CAUSE_ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED                   (84)
#define NW_GTPV2C_CAUSE_UE_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED             (85)
#define NW_GTPV2C_CAUSE_PROTOCOL_TYPE_NOT_SUPPORTED                          (86)
#define NW_GTPV2C_CAUSE_UE_NOT_RESPONDING                                    (87)
#define NW_GTPV2C_CAUSE_UE_REFUSES                                           (88)
#define NW_GTPV2C_CAUSE_SERVICE_DENIED                                       (89)
#define NW_GTPV2C_CAUSE_UNABLE_TO_PAGE_UE                                    (90)
#define NW_GTPV2C_CAUSE_NO_MEMORY_AVAILABLE                                  (91)
#define NW_GTPV2C_CAUSE_USER_AUTHENTICATION_FAILED                           (92)
#define NW_GTPV2C_CAUSE_APN_ACCESS_DENIED_NO_SUBSCRIPTION                    (93)
#define NW_GTPV2C_CAUSE_REQUEST_REJECTED                                     (94)
#define NW_GTPV2C_CAUSE_P_TMSI_SIGNATURE_MISMATCH                            (95)
#define NW_GTPV2C_CAUSE_IMSI_IMEI_NOT_KNOWN                                  (96)
#define NW_GTPV2C_CAUSE_SEMANTIC_ERROR_IN_THE_TAD_OPERATION                  (97)
#define NW_GTPV2C_CAUSE_SYNTACTIC_ERROR_IN_THE_TAD_OPERATION                 (98)
#define NW_GTPV2C_CAUSE_REMOTE_PEER_NOT_RESPONDING                           (100)
#define NW_GTPV2C_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST             (101)
#define NW_GTPV2C_CAUSE_UNABLE_TO_PAGE_UE_DUE_TO_SUSPENSION                  (102)
#define NW_GTPV2C_CAUSE_CONDITIONAL_IE_MISSING                               (103)
#define NW_GTPV2C_CAUSE_APN_RESTRICTION_TYPE_INCOMPATIBLE_WITH_CURRENTLY_ACTIVE_PDN_CONNECTION    (104)
#define NW_GTPV2C_CAUSE_INVALID_OVERALL_LENGTH_OF_THE_TRIGGERED_RESPONSE_MESSAGE_AND_A_PIGGYBACKED_INITIAL_MESSAGE    (105)
#define NW_GTPV2C_CAUSE_DATA_FORWARDING_NOT_SUPPORTED                        (106)
#define NW_GTPV2C_CAUSE_INVALID_REPLY_FROM_REMOTE_PEER                       (107)
#define NW_GTPV2C_CAUSE_FALLBACK_TO_GTPV1                                    (108)
#define NW_GTPV2C_CAUSE_INVALID_PEER                                         (109)
#define NW_GTPV2C_CAUSE_TEMPORARILY_REJECTED_DUE_TO_HANDOVER_PROCEDURE_IN_PROGRESS    (110)
#define NW_GTPV2C_CAUSE_MODIFICATIONS_NOT_LIMITED_TO_S1_U_BEARERS            (111)
#define NW_GTPV2C_CAUSE_REQUEST_REJECTED_FOR_A_PMIPV6_REASON                 (112)
#define NW_GTPV2C_CAUSE_APN_CONGESTION                                       (113)
#define NW_GTPV2C_CAUSE_BEARER_HANDLING_NOT_SUPPORTED                        (114)
#define NW_GTPV2C_CAUSE_UE_ALREADY_RE_ATTACHED                               (115)
#define NW_GTPV2C_CAUSE_MULTIPLE_PDN_CONNECTIONS_FOR_A_GIVEN_APN_NOT_ALLOWED (116)


#define NW_GTPV2C_IE_INSTANCE_ZERO                                      (0)
#define NW_GTPV2C_IE_INSTANCE_ONE                                       (1)
#define NW_GTPV2C_IE_INSTANCE_TWO                                       (2)
#define NW_GTPV2C_IE_INSTANCE_THREE                                     (3)
#define NW_GTPV2C_IE_INSTANCE_FOUR                                      (4)
#define NW_GTPV2C_IE_INSTANCE_MAXIMUM                                   (NW_GTPV2C_IE_INSTANCE_FOUR)

#define NW_GTPV2C_IE_PRESENCE_MANDATORY                                 (1)
#define NW_GTPV2C_IE_PRESENCE_CONDITIONAL                               (2)
#define NW_GTPV2C_IE_PRESENCE_CONDITIONAL_OPTIONAL                      (3)
#define NW_GTPV2C_IE_PRESENCE_OPTIONAL                                  (4)

#endif  /* __NW_GTPV2C_IE_H__ */

/*--------------------------------------------------------------------------*
 *                      E N D     O F    F I L E                            *
 *--------------------------------------------------------------------------*/
