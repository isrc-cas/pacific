/* Please make sure you have ELF spec for reference when reading source code
 *   http://www.skyfree.org/linux/references/ELF_Format.pdf
 *
 * ELF related macros and data structures are borrowed from GNU Binutils:
 *  git://sourceware.org/git/binutils-gdb.git
 *
 */
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

enum {
  R_AARCH64_NONE = 0,
  R_AARCH64_P32_ABS32 = 1,
  R_AARCH64_P32_ABS16 = 2,
  R_AARCH64_P32_PREL32 = 3,
  R_AARCH64_P32_PREL16 = 4,
  R_AARCH64_P32_MOVW_UABS_G0 = 5,
  R_AARCH64_P32_MOVW_UABS_G0_NC = 6,
  R_AARCH64_P32_MOVW_UABS_G1 = 7,
  R_AARCH64_P32_MOVW_SABS_G0 = 8,
  R_AARCH64_P32_LD_PREL_LO19 = 9,
  R_AARCH64_P32_ADR_PREL_LO21 = 10,
  R_AARCH64_P32_ADR_PREL_PG_HI21 = 11,
  R_AARCH64_P32_ADD_ABS_LO12_NC = 12,
  R_AARCH64_P32_LDST8_ABS_LO12_NC = 13,
  R_AARCH64_P32_LDST16_ABS_LO12_NC = 14,
  R_AARCH64_P32_LDST32_ABS_LO12_NC = 15,
  R_AARCH64_P32_LDST64_ABS_LO12_NC = 16,
  R_AARCH64_P32_LDST128_ABS_LO12_NC = 17,
  R_AARCH64_P32_TSTBR14 = 18,
  R_AARCH64_P32_CONDBR19 = 19,
  R_AARCH64_P32_JUMP26 = 20,
  R_AARCH64_P32_CALL26 = 21,
  R_AARCH64_P32_MOVW_PREL_G0 = 22,
  R_AARCH64_P32_MOVW_PREL_G0_NC = 23,
  R_AARCH64_P32_MOVW_PREL_G1 = 24,
  R_AARCH64_P32_GOT_LD_PREL19 = 25,
  R_AARCH64_P32_ADR_GOT_PAGE = 26,
  R_AARCH64_P32_LD32_GOT_LO12_NC = 27,
  R_AARCH64_P32_LD32_GOTPAGE_LO14 = 28,
  R_AARCH64_P32_TLSGD_ADR_PREL21 = 80,
  R_AARCH64_P32_TLSGD_ADR_PAGE21 = 81,
  R_AARCH64_P32_TLSGD_ADD_LO12_NC = 82,
  R_AARCH64_P32_TLSLD_ADR_PREL21 = 83,
  R_AARCH64_P32_TLSLD_ADR_PAGE21 = 84,
  R_AARCH64_P32_TLSLD_ADD_LO12_NC = 85,
  R_AARCH64_P32_TLSLD_MOVW_DTPREL_G1 = 87,
  R_AARCH64_P32_TLSLD_MOVW_DTPREL_G0 = 88,
  R_AARCH64_P32_TLSLD_MOVW_DTPREL_G0_NC = 89,
  R_AARCH64_P32_TLSLD_ADD_DTPREL_HI12 = 90,
  R_AARCH64_P32_TLSLD_ADD_DTPREL_LO12 = 91,
  R_AARCH64_P32_TLSLD_ADD_DTPREL_LO12_NC = 92,
  R_AARCH64_P32_TLSIE_ADR_GOTTPREL_PAGE21 = 103,
  R_AARCH64_P32_TLSIE_LD32_GOTTPREL_LO12_NC = 104,
  R_AARCH64_P32_TLSIE_LD_GOTTPREL_PREL19 = 105,
  R_AARCH64_P32_TLSLE_MOVW_TPREL_G1 = 106,
  R_AARCH64_P32_TLSLE_MOVW_TPREL_G0 = 107,
  R_AARCH64_P32_TLSLE_MOVW_TPREL_G0_NC = 108,
  R_AARCH64_P32_TLSLE_ADD_TPREL_HI12 = 109,
  R_AARCH64_P32_TLSLE_ADD_TPREL_LO12 = 110,
  R_AARCH64_P32_TLSLE_ADD_TPREL_LO12_NC = 111,
  R_AARCH64_P32_TLSLE_LDST8_TPREL_LO12 = 112,
  R_AARCH64_P32_TLSLE_LDST8_TPREL_LO12_NC = 113,
  R_AARCH64_P32_TLSLE_LDST16_TPREL_LO12 = 114,
  R_AARCH64_P32_TLSLE_LDST16_TPREL_LO12_NC = 115,
  R_AARCH64_P32_TLSLE_LDST32_TPREL_LO12 = 116,
  R_AARCH64_P32_TLSLE_LDST32_TPREL_LO12_NC = 117,
  R_AARCH64_P32_TLSLE_LDST64_TPREL_LO12 = 118,
  R_AARCH64_P32_TLSLE_LDST64_TPREL_LO12_NC = 119,
  R_AARCH64_P32_TLSDESC_LD_PREL19 = 122,
  R_AARCH64_P32_TLSDESC_ADR_PREL21 = 123,
  R_AARCH64_P32_TLSDESC_ADR_PAGE21 = 124,
  R_AARCH64_P32_TLSDESC_LD32_LO12_NC = 125,
  R_AARCH64_P32_TLSDESC_ADD_LO12_NC = 126,
  R_AARCH64_P32_TLSDESC_CALL = 127,
  R_AARCH64_P32_COPY = 180,
  R_AARCH64_P32_GLOB_DAT = 181,
  R_AARCH64_P32_JUMP_SLOT = 182,
  R_AARCH64_P32_RELATIVE = 183,
  R_AARCH64_P32_TLS_DTPMOD = 184,
  R_AARCH64_P32_TLS_DTPREL = 185,
  R_AARCH64_P32_TLS_TPREL = 186,
  R_AARCH64_P32_TLSDESC = 187,
  R_AARCH64_P32_IRELATIVE = 188,
  R_AARCH64_NULL = 256,
  R_AARCH64_ABS64 = 257,
  R_AARCH64_ABS32 = 258,
  R_AARCH64_ABS16 = 259,
  R_AARCH64_PREL64 = 260,
  R_AARCH64_PREL32 = 261,
  R_AARCH64_PREL16 = 262,
  R_AARCH64_MOVW_UABS_G0 = 263,
  R_AARCH64_MOVW_UABS_G0_NC = 264,
  R_AARCH64_MOVW_UABS_G1 = 265,
  R_AARCH64_MOVW_UABS_G1_NC = 266,
  R_AARCH64_MOVW_UABS_G2 = 267,
  R_AARCH64_MOVW_UABS_G2_NC = 268,
  R_AARCH64_MOVW_UABS_G3 = 269,
  R_AARCH64_MOVW_SABS_G0 = 270,
  R_AARCH64_MOVW_SABS_G1 = 271,
  R_AARCH64_MOVW_SABS_G2 = 272,
  R_AARCH64_LD_PREL_LO19 = 273,
  R_AARCH64_ADR_PREL_LO21 = 274,
  R_AARCH64_ADR_PREL_PG_HI21 = 275,
  R_AARCH64_ADR_PREL_PG_HI21_NC = 276,
  R_AARCH64_ADD_ABS_LO12_NC = 277,
  R_AARCH64_LDST8_ABS_LO12_NC = 278,
  R_AARCH64_TSTBR14 = 279,
  R_AARCH64_CONDBR19 = 280,
  R_AARCH64_JUMP26 = 282,
  R_AARCH64_CALL26 = 283,
  R_AARCH64_LDST16_ABS_LO12_NC = 284,
  R_AARCH64_LDST32_ABS_LO12_NC = 285,
  R_AARCH64_LDST64_ABS_LO12_NC = 286,
  R_AARCH64_MOVW_PREL_G0 = 287,
  R_AARCH64_MOVW_PREL_G0_NC = 288,
  R_AARCH64_MOVW_PREL_G1 = 289,
  R_AARCH64_MOVW_PREL_G1_NC = 290,
  R_AARCH64_MOVW_PREL_G2 = 291,
  R_AARCH64_MOVW_PREL_G2_NC = 292,
  R_AARCH64_MOVW_PREL_G3 = 293,
  R_AARCH64_LDST128_ABS_LO12_NC = 299,
  R_AARCH64_MOVW_GOTOFF_G0 = 300,
  R_AARCH64_MOVW_GOTOFF_G0_NC = 301,
  R_AARCH64_MOVW_GOTOFF_G1 = 302,
  R_AARCH64_MOVW_GOTOFF_G1_NC = 303,
  R_AARCH64_MOVW_GOTOFF_G2 = 304,
  R_AARCH64_MOVW_GOTOFF_G2_NC = 305,
  R_AARCH64_MOVW_GOTOFF_G3 = 306,
  R_AARCH64_GOTREL64 = 307,
  R_AARCH64_GOTREL32 = 308,
  R_AARCH64_GOT_LD_PREL19 = 309,
  R_AARCH64_LD64_GOTOFF_LO15 = 310,
  R_AARCH64_ADR_GOT_PAGE = 311,
  R_AARCH64_LD64_GOT_LO12_NC = 312,
  R_AARCH64_LD64_GOTPAGE_LO15 = 313,
  R_AARCH64_TLSGD_ADR_PREL21 = 512,
  R_AARCH64_TLSGD_ADR_PAGE21 = 513,
  R_AARCH64_TLSGD_ADD_LO12_NC = 514,
  R_AARCH64_TLSGD_MOVW_G1 = 515,
  R_AARCH64_TLSGD_MOVW_G0_NC = 516,
  R_AARCH64_TLSLD_ADR_PREL21 = 517,
  R_AARCH64_TLSLD_ADR_PAGE21 = 518,
  R_AARCH64_TLSLD_ADD_LO12_NC = 519,
  R_AARCH64_TLSLD_MOVW_G1 = 520,
  R_AARCH64_TLSLD_MOVW_G0_NC = 521,
  R_AARCH64_TLSLD_LD_PREL19 = 522,
  R_AARCH64_TLSLD_MOVW_DTPREL_G2 = 523,
  R_AARCH64_TLSLD_MOVW_DTPREL_G1 = 524,
  R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC = 525,
  R_AARCH64_TLSLD_MOVW_DTPREL_G0 = 526,
  R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC = 527,
  R_AARCH64_TLSLD_ADD_DTPREL_HI12 = 528,
  R_AARCH64_TLSLD_ADD_DTPREL_LO12 = 529,
  R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC = 530,
  R_AARCH64_TLSLD_LDST8_DTPREL_LO12 = 531,
  R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC = 532,
  R_AARCH64_TLSLD_LDST16_DTPREL_LO12 = 533,
  R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC = 534,
  R_AARCH64_TLSLD_LDST32_DTPREL_LO12 = 535,
  R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC = 536,
  R_AARCH64_TLSLD_LDST64_DTPREL_LO12 = 537,
  R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC = 538,
  R_AARCH64_TLSIE_MOVW_GOTTPREL_G1 = 539,
  R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC = 540,
  R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21 = 541,
  R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC = 542,
  R_AARCH64_TLSIE_LD_GOTTPREL_PREL19 = 543,
  R_AARCH64_TLSLE_MOVW_TPREL_G2 = 544,
  R_AARCH64_TLSLE_MOVW_TPREL_G1 = 545,
  R_AARCH64_TLSLE_MOVW_TPREL_G1_NC = 546,
  R_AARCH64_TLSLE_MOVW_TPREL_G0 = 547,
  R_AARCH64_TLSLE_MOVW_TPREL_G0_NC = 548,
  R_AARCH64_TLSLE_ADD_TPREL_HI12 = 549,
  R_AARCH64_TLSLE_ADD_TPREL_LO12 = 550,
  R_AARCH64_TLSLE_ADD_TPREL_LO12_NC = 551,
  R_AARCH64_TLSLE_LDST8_TPREL_LO12 = 552,
  R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC = 553,
  R_AARCH64_TLSLE_LDST16_TPREL_LO12 = 554,
  R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC = 555,
  R_AARCH64_TLSLE_LDST32_TPREL_LO12 = 556,
  R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC = 557,
  R_AARCH64_TLSLE_LDST64_TPREL_LO12 = 558,
  R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC = 559,
  R_AARCH64_TLSDESC_LD_PREL19 = 560,
  R_AARCH64_TLSDESC_ADR_PREL21 = 561,
  R_AARCH64_TLSDESC_ADR_PAGE21 = 562,
  R_AARCH64_TLSDESC_LD64_LO12 = 563,
  R_AARCH64_TLSDESC_ADD_LO12 = 564,
  R_AARCH64_TLSDESC_OFF_G1 = 565,
  R_AARCH64_TLSDESC_OFF_G0_NC = 566,
  R_AARCH64_TLSDESC_LDR = 567,
  R_AARCH64_TLSDESC_ADD = 568,
  R_AARCH64_TLSDESC_CALL = 569,
  R_AARCH64_TLSLE_LDST128_TPREL_LO12 = 570,
  R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC = 571,
  R_AARCH64_TLSLD_LDST128_DTPREL_LO12 = 572,
  R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC = 573,
  R_AARCH64_COPY = 1024,
  R_AARCH64_GLOB_DAT = 1025,
  R_AARCH64_JUMP_SLOT = 1026,
  R_AARCH64_RELATIVE = 1027,
  R_AARCH64_TLS_DTPMOD64 = 1028,
  R_AARCH64_TLS_DTPREL64 = 1029,
  R_AARCH64_TLS_TPREL64 = 1030,
  R_AARCH64_TLS_DTPMOD = 1028,
  R_AARCH64_TLS_DTPREL = 1029,
  R_AARCH64_TLS_TPREL = 1030,
  R_AARCH64_TLSDESC = 1031,
  R_AARCH64_IRELATIVE = 1032,
  R_AARCH64_RELOC_NUM = R_AARCH64_IRELATIVE + 1,
};

static const char *reloc_name[R_AARCH64_RELOC_NUM];

static void init_reloc_name()
{
  reloc_name[R_AARCH64_NONE] = "R_AARCH64_NONE";
  reloc_name[R_AARCH64_P32_ABS32] = "R_AARCH64_P32_ABS32";
  reloc_name[R_AARCH64_P32_ABS16] = "R_AARCH64_P32_ABS16";
  reloc_name[R_AARCH64_P32_PREL32] = "R_AARCH64_P32_PREL32";
  reloc_name[R_AARCH64_P32_PREL16] = "R_AARCH64_P32_PREL16";
  reloc_name[R_AARCH64_P32_MOVW_UABS_G0] = "R_AARCH64_P32_MOVW_UABS_G0";
  reloc_name[R_AARCH64_P32_MOVW_UABS_G0_NC] = "R_AARCH64_P32_MOVW_UABS_G0_NC";
  reloc_name[R_AARCH64_P32_MOVW_UABS_G1] = "R_AARCH64_P32_MOVW_UABS_G1";
  reloc_name[R_AARCH64_P32_MOVW_SABS_G0] = "R_AARCH64_P32_MOVW_SABS_G0";
  reloc_name[R_AARCH64_P32_LD_PREL_LO19] = "R_AARCH64_P32_LD_PREL_LO19";
  reloc_name[R_AARCH64_P32_ADR_PREL_LO21] = "R_AARCH64_P32_ADR_PREL_LO21";
  reloc_name[R_AARCH64_P32_ADR_PREL_PG_HI21] = "R_AARCH64_P32_ADR_PREL_PG_HI21";
  reloc_name[R_AARCH64_P32_ADD_ABS_LO12_NC] = "R_AARCH64_P32_ADD_ABS_LO12_NC";
  reloc_name[R_AARCH64_P32_LDST8_ABS_LO12_NC] = "R_AARCH64_P32_LDST8_ABS_LO12_NC";
  reloc_name[R_AARCH64_P32_LDST16_ABS_LO12_NC] = "R_AARCH64_P32_LDST16_ABS_LO12_NC";
  reloc_name[R_AARCH64_P32_LDST32_ABS_LO12_NC] = "R_AARCH64_P32_LDST32_ABS_LO12_NC";
  reloc_name[R_AARCH64_P32_LDST64_ABS_LO12_NC] = "R_AARCH64_P32_LDST64_ABS_LO12_NC";
  reloc_name[R_AARCH64_P32_LDST128_ABS_LO12_NC] = "R_AARCH64_P32_LDST128_ABS_LO12_NC";
  reloc_name[R_AARCH64_P32_TSTBR14] = "R_AARCH64_P32_TSTBR14";
  reloc_name[R_AARCH64_P32_CONDBR19] = "R_AARCH64_P32_CONDBR19";
  reloc_name[R_AARCH64_P32_JUMP26] = "R_AARCH64_P32_JUMP26";
  reloc_name[R_AARCH64_P32_CALL26] = "R_AARCH64_P32_CALL26";
  reloc_name[R_AARCH64_P32_MOVW_PREL_G0] = "R_AARCH64_P32_MOVW_PREL_G0";
  reloc_name[R_AARCH64_P32_MOVW_PREL_G0_NC] = "R_AARCH64_P32_MOVW_PREL_G0_NC";
  reloc_name[R_AARCH64_P32_MOVW_PREL_G1] = "R_AARCH64_P32_MOVW_PREL_G1";
  reloc_name[R_AARCH64_P32_GOT_LD_PREL19] = "R_AARCH64_P32_GOT_LD_PREL19";
  reloc_name[R_AARCH64_P32_ADR_GOT_PAGE] = "R_AARCH64_P32_ADR_GOT_PAGE";
  reloc_name[R_AARCH64_P32_LD32_GOT_LO12_NC] = "R_AARCH64_P32_LD32_GOT_LO12_NC";
  reloc_name[R_AARCH64_P32_LD32_GOTPAGE_LO14] = "R_AARCH64_P32_LD32_GOTPAGE_LO14";
  reloc_name[R_AARCH64_P32_TLSGD_ADR_PREL21] = "R_AARCH64_P32_TLSGD_ADR_PREL21";
  reloc_name[R_AARCH64_P32_TLSGD_ADR_PAGE21] = "R_AARCH64_P32_TLSGD_ADR_PAGE21";
  reloc_name[R_AARCH64_P32_TLSGD_ADD_LO12_NC] = "R_AARCH64_P32_TLSGD_ADD_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSLD_ADR_PREL21] = "R_AARCH64_P32_TLSLD_ADR_PREL21";
  reloc_name[R_AARCH64_P32_TLSLD_ADR_PAGE21] = "R_AARCH64_P32_TLSLD_ADR_PAGE21";
  reloc_name[R_AARCH64_P32_TLSLD_ADD_LO12_NC] = "R_AARCH64_P32_TLSLD_ADD_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSLD_MOVW_DTPREL_G1] = "R_AARCH64_P32_TLSLD_MOVW_DTPREL_G1";
  reloc_name[R_AARCH64_P32_TLSLD_MOVW_DTPREL_G0] = "R_AARCH64_P32_TLSLD_MOVW_DTPREL_G0";
  reloc_name[R_AARCH64_P32_TLSLD_MOVW_DTPREL_G0_NC] = "R_AARCH64_P32_TLSLD_MOVW_DTPREL_G0_NC";
  reloc_name[R_AARCH64_P32_TLSLD_ADD_DTPREL_HI12] = "R_AARCH64_P32_TLSLD_ADD_DTPREL_HI12";
  reloc_name[R_AARCH64_P32_TLSLD_ADD_DTPREL_LO12] = "R_AARCH64_P32_TLSLD_ADD_DTPREL_LO12";
  reloc_name[R_AARCH64_P32_TLSLD_ADD_DTPREL_LO12_NC] = "R_AARCH64_P32_TLSLD_ADD_DTPREL_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSIE_ADR_GOTTPREL_PAGE21] = "R_AARCH64_P32_TLSIE_ADR_GOTTPREL_PAGE21";
  reloc_name[R_AARCH64_P32_TLSIE_LD32_GOTTPREL_LO12_NC] = "R_AARCH64_P32_TLSIE_LD32_GOTTPREL_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSIE_LD_GOTTPREL_PREL19] = "R_AARCH64_P32_TLSIE_LD_GOTTPREL_PREL19";
  reloc_name[R_AARCH64_P32_TLSLE_MOVW_TPREL_G1] = "R_AARCH64_P32_TLSLE_MOVW_TPREL_G1";
  reloc_name[R_AARCH64_P32_TLSLE_MOVW_TPREL_G0] = "R_AARCH64_P32_TLSLE_MOVW_TPREL_G0";
  reloc_name[R_AARCH64_P32_TLSLE_MOVW_TPREL_G0_NC] = "R_AARCH64_P32_TLSLE_MOVW_TPREL_G0_NC";
  reloc_name[R_AARCH64_P32_TLSLE_ADD_TPREL_HI12] = "R_AARCH64_P32_TLSLE_ADD_TPREL_HI12";
  reloc_name[R_AARCH64_P32_TLSLE_ADD_TPREL_LO12] = "R_AARCH64_P32_TLSLE_ADD_TPREL_LO12";
  reloc_name[R_AARCH64_P32_TLSLE_ADD_TPREL_LO12_NC] = "R_AARCH64_P32_TLSLE_ADD_TPREL_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSLE_LDST8_TPREL_LO12] = "R_AARCH64_P32_TLSLE_LDST8_TPREL_LO12";
  reloc_name[R_AARCH64_P32_TLSLE_LDST8_TPREL_LO12_NC] = "R_AARCH64_P32_TLSLE_LDST8_TPREL_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSLE_LDST16_TPREL_LO12] = "R_AARCH64_P32_TLSLE_LDST16_TPREL_LO12";
  reloc_name[R_AARCH64_P32_TLSLE_LDST16_TPREL_LO12_NC] = "R_AARCH64_P32_TLSLE_LDST16_TPREL_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSLE_LDST32_TPREL_LO12] = "R_AARCH64_P32_TLSLE_LDST32_TPREL_LO12";
  reloc_name[R_AARCH64_P32_TLSLE_LDST32_TPREL_LO12_NC] = "R_AARCH64_P32_TLSLE_LDST32_TPREL_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSLE_LDST64_TPREL_LO12] = "R_AARCH64_P32_TLSLE_LDST64_TPREL_LO12";
  reloc_name[R_AARCH64_P32_TLSLE_LDST64_TPREL_LO12_NC] = "R_AARCH64_P32_TLSLE_LDST64_TPREL_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSDESC_LD_PREL19] = "R_AARCH64_P32_TLSDESC_LD_PREL19";
  reloc_name[R_AARCH64_P32_TLSDESC_ADR_PREL21] = "R_AARCH64_P32_TLSDESC_ADR_PREL21";
  reloc_name[R_AARCH64_P32_TLSDESC_ADR_PAGE21] = "R_AARCH64_P32_TLSDESC_ADR_PAGE21";
  reloc_name[R_AARCH64_P32_TLSDESC_LD32_LO12_NC] = "R_AARCH64_P32_TLSDESC_LD32_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSDESC_ADD_LO12_NC] = "R_AARCH64_P32_TLSDESC_ADD_LO12_NC";
  reloc_name[R_AARCH64_P32_TLSDESC_CALL] = "R_AARCH64_P32_TLSDESC_CALL";
  reloc_name[R_AARCH64_P32_COPY] = "R_AARCH64_P32_COPY";
  reloc_name[R_AARCH64_P32_GLOB_DAT] = "R_AARCH64_P32_GLOB_DAT";
  reloc_name[R_AARCH64_P32_JUMP_SLOT] = "R_AARCH64_P32_JUMP_SLOT";
  reloc_name[R_AARCH64_P32_RELATIVE] = "R_AARCH64_P32_RELATIVE";
  reloc_name[R_AARCH64_P32_TLS_DTPMOD] = "R_AARCH64_P32_TLS_DTPMOD";
  reloc_name[R_AARCH64_P32_TLS_DTPREL] = "R_AARCH64_P32_TLS_DTPREL";
  reloc_name[R_AARCH64_P32_TLS_TPREL] = "R_AARCH64_P32_TLS_TPREL";
  reloc_name[R_AARCH64_P32_TLSDESC] = "R_AARCH64_P32_TLSDESC";
  reloc_name[R_AARCH64_P32_IRELATIVE] = "R_AARCH64_P32_IRELATIVE";
  reloc_name[R_AARCH64_NULL] = "R_AARCH64_NULL";
  reloc_name[R_AARCH64_ABS64] = "R_AARCH64_ABS64";
  reloc_name[R_AARCH64_ABS32] = "R_AARCH64_ABS32";
  reloc_name[R_AARCH64_ABS16] = "R_AARCH64_ABS16";
  reloc_name[R_AARCH64_PREL64] = "R_AARCH64_PREL64";
  reloc_name[R_AARCH64_PREL32] = "R_AARCH64_PREL32";
  reloc_name[R_AARCH64_PREL16] = "R_AARCH64_PREL16";
  reloc_name[R_AARCH64_MOVW_UABS_G0] = "R_AARCH64_MOVW_UABS_G0";
  reloc_name[R_AARCH64_MOVW_UABS_G0_NC] = "R_AARCH64_MOVW_UABS_G0_NC";
  reloc_name[R_AARCH64_MOVW_UABS_G1] = "R_AARCH64_MOVW_UABS_G1";
  reloc_name[R_AARCH64_MOVW_UABS_G1_NC] = "R_AARCH64_MOVW_UABS_G1_NC";
  reloc_name[R_AARCH64_MOVW_UABS_G2] = "R_AARCH64_MOVW_UABS_G2";
  reloc_name[R_AARCH64_MOVW_UABS_G2_NC] = "R_AARCH64_MOVW_UABS_G2_NC";
  reloc_name[R_AARCH64_MOVW_UABS_G3] = "R_AARCH64_MOVW_UABS_G3";
  reloc_name[R_AARCH64_MOVW_SABS_G0] = "R_AARCH64_MOVW_SABS_G0";
  reloc_name[R_AARCH64_MOVW_SABS_G1] = "R_AARCH64_MOVW_SABS_G1";
  reloc_name[R_AARCH64_MOVW_SABS_G2] = "R_AARCH64_MOVW_SABS_G2";
  reloc_name[R_AARCH64_LD_PREL_LO19] = "R_AARCH64_LD_PREL_LO19";
  reloc_name[R_AARCH64_ADR_PREL_LO21] = "R_AARCH64_ADR_PREL_LO21";
  reloc_name[R_AARCH64_ADR_PREL_PG_HI21] = "R_AARCH64_ADR_PREL_PG_HI21";
  reloc_name[R_AARCH64_ADR_PREL_PG_HI21_NC] = "R_AARCH64_ADR_PREL_PG_HI21_NC";
  reloc_name[R_AARCH64_ADD_ABS_LO12_NC] = "R_AARCH64_ADD_ABS_LO12_NC";
  reloc_name[R_AARCH64_LDST8_ABS_LO12_NC] = "R_AARCH64_LDST8_ABS_LO12_NC";
  reloc_name[R_AARCH64_TSTBR14] = "R_AARCH64_TSTBR14";
  reloc_name[R_AARCH64_CONDBR19] = "R_AARCH64_CONDBR19";
  reloc_name[R_AARCH64_JUMP26] = "R_AARCH64_JUMP26";
  reloc_name[R_AARCH64_CALL26] = "R_AARCH64_CALL26";
  reloc_name[R_AARCH64_LDST16_ABS_LO12_NC] = "R_AARCH64_LDST16_ABS_LO12_NC";
  reloc_name[R_AARCH64_LDST32_ABS_LO12_NC] = "R_AARCH64_LDST32_ABS_LO12_NC";
  reloc_name[R_AARCH64_LDST64_ABS_LO12_NC] = "R_AARCH64_LDST64_ABS_LO12_NC";
  reloc_name[R_AARCH64_MOVW_PREL_G0] = "R_AARCH64_MOVW_PREL_G0";
  reloc_name[R_AARCH64_MOVW_PREL_G0_NC] = "R_AARCH64_MOVW_PREL_G0_NC";
  reloc_name[R_AARCH64_MOVW_PREL_G1] = "R_AARCH64_MOVW_PREL_G1";
  reloc_name[R_AARCH64_MOVW_PREL_G1_NC] = "R_AARCH64_MOVW_PREL_G1_NC";
  reloc_name[R_AARCH64_MOVW_PREL_G2] = "R_AARCH64_MOVW_PREL_G2";
  reloc_name[R_AARCH64_MOVW_PREL_G2_NC] = "R_AARCH64_MOVW_PREL_G2_NC";
  reloc_name[R_AARCH64_MOVW_PREL_G3] = "R_AARCH64_MOVW_PREL_G3";
  reloc_name[R_AARCH64_LDST128_ABS_LO12_NC] = "R_AARCH64_LDST128_ABS_LO12_NC";
  reloc_name[R_AARCH64_MOVW_GOTOFF_G0] = "R_AARCH64_MOVW_GOTOFF_G0";
  reloc_name[R_AARCH64_MOVW_GOTOFF_G0_NC] = "R_AARCH64_MOVW_GOTOFF_G0_NC";
  reloc_name[R_AARCH64_MOVW_GOTOFF_G1] = "R_AARCH64_MOVW_GOTOFF_G1";
  reloc_name[R_AARCH64_MOVW_GOTOFF_G1_NC] = "R_AARCH64_MOVW_GOTOFF_G1_NC";
  reloc_name[R_AARCH64_MOVW_GOTOFF_G2] = "R_AARCH64_MOVW_GOTOFF_G2";
  reloc_name[R_AARCH64_MOVW_GOTOFF_G2_NC] = "R_AARCH64_MOVW_GOTOFF_G2_NC";
  reloc_name[R_AARCH64_MOVW_GOTOFF_G3] = "R_AARCH64_MOVW_GOTOFF_G3";
  reloc_name[R_AARCH64_GOTREL64] = "R_AARCH64_GOTREL64";
  reloc_name[R_AARCH64_GOTREL32] = "R_AARCH64_GOTREL32";
  reloc_name[R_AARCH64_GOT_LD_PREL19] = "R_AARCH64_GOT_LD_PREL19";
  reloc_name[R_AARCH64_LD64_GOTOFF_LO15] = "R_AARCH64_LD64_GOTOFF_LO15";
  reloc_name[R_AARCH64_ADR_GOT_PAGE] = "R_AARCH64_ADR_GOT_PAGE";
  reloc_name[R_AARCH64_LD64_GOT_LO12_NC] = "R_AARCH64_LD64_GOT_LO12_NC";
  reloc_name[R_AARCH64_LD64_GOTPAGE_LO15] = "R_AARCH64_LD64_GOTPAGE_LO15";
  reloc_name[R_AARCH64_TLSGD_ADR_PREL21] = "R_AARCH64_TLSGD_ADR_PREL21";
  reloc_name[R_AARCH64_TLSGD_ADR_PAGE21] = "R_AARCH64_TLSGD_ADR_PAGE21";
  reloc_name[R_AARCH64_TLSGD_ADD_LO12_NC] = "R_AARCH64_TLSGD_ADD_LO12_NC";
  reloc_name[R_AARCH64_TLSGD_MOVW_G1] = "R_AARCH64_TLSGD_MOVW_G1";
  reloc_name[R_AARCH64_TLSGD_MOVW_G0_NC] = "R_AARCH64_TLSGD_MOVW_G0_NC";
  reloc_name[R_AARCH64_TLSLD_ADR_PREL21] = "R_AARCH64_TLSLD_ADR_PREL21";
  reloc_name[R_AARCH64_TLSLD_ADR_PAGE21] = "R_AARCH64_TLSLD_ADR_PAGE21";
  reloc_name[R_AARCH64_TLSLD_ADD_LO12_NC] = "R_AARCH64_TLSLD_ADD_LO12_NC";
  reloc_name[R_AARCH64_TLSLD_MOVW_G1] = "R_AARCH64_TLSLD_MOVW_G1";
  reloc_name[R_AARCH64_TLSLD_MOVW_G0_NC] = "R_AARCH64_TLSLD_MOVW_G0_NC";
  reloc_name[R_AARCH64_TLSLD_LD_PREL19] = "R_AARCH64_TLSLD_LD_PREL19";
  reloc_name[R_AARCH64_TLSLD_MOVW_DTPREL_G2] = "R_AARCH64_TLSLD_MOVW_DTPREL_G2";
  reloc_name[R_AARCH64_TLSLD_MOVW_DTPREL_G1] = "R_AARCH64_TLSLD_MOVW_DTPREL_G1";
  reloc_name[R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC] = "R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC";
  reloc_name[R_AARCH64_TLSLD_MOVW_DTPREL_G0] = "R_AARCH64_TLSLD_MOVW_DTPREL_G0";
  reloc_name[R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC] = "R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC";
  reloc_name[R_AARCH64_TLSLD_ADD_DTPREL_HI12] = "R_AARCH64_TLSLD_ADD_DTPREL_HI12";
  reloc_name[R_AARCH64_TLSLD_ADD_DTPREL_LO12] = "R_AARCH64_TLSLD_ADD_DTPREL_LO12";
  reloc_name[R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC] = "R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLD_LDST8_DTPREL_LO12] = "R_AARCH64_TLSLD_LDST8_DTPREL_LO12";
  reloc_name[R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC] = "R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLD_LDST16_DTPREL_LO12] = "R_AARCH64_TLSLD_LDST16_DTPREL_LO12";
  reloc_name[R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC] = "R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLD_LDST32_DTPREL_LO12] = "R_AARCH64_TLSLD_LDST32_DTPREL_LO12";
  reloc_name[R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC] = "R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLD_LDST64_DTPREL_LO12] = "R_AARCH64_TLSLD_LDST64_DTPREL_LO12";
  reloc_name[R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC] = "R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSIE_MOVW_GOTTPREL_G1] = "R_AARCH64_TLSIE_MOVW_GOTTPREL_G1";
  reloc_name[R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC] = "R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC";
  reloc_name[R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21] = "R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21";
  reloc_name[R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC] = "R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSIE_LD_GOTTPREL_PREL19] = "R_AARCH64_TLSIE_LD_GOTTPREL_PREL19";
  reloc_name[R_AARCH64_TLSLE_MOVW_TPREL_G2] = "R_AARCH64_TLSLE_MOVW_TPREL_G2";
  reloc_name[R_AARCH64_TLSLE_MOVW_TPREL_G1] = "R_AARCH64_TLSLE_MOVW_TPREL_G1";
  reloc_name[R_AARCH64_TLSLE_MOVW_TPREL_G1_NC] = "R_AARCH64_TLSLE_MOVW_TPREL_G1_NC";
  reloc_name[R_AARCH64_TLSLE_MOVW_TPREL_G0] = "R_AARCH64_TLSLE_MOVW_TPREL_G0";
  reloc_name[R_AARCH64_TLSLE_MOVW_TPREL_G0_NC] = "R_AARCH64_TLSLE_MOVW_TPREL_G0_NC";
  reloc_name[R_AARCH64_TLSLE_ADD_TPREL_HI12] = "R_AARCH64_TLSLE_ADD_TPREL_HI12";
  reloc_name[R_AARCH64_TLSLE_ADD_TPREL_LO12] = "R_AARCH64_TLSLE_ADD_TPREL_LO12";
  reloc_name[R_AARCH64_TLSLE_ADD_TPREL_LO12_NC] = "R_AARCH64_TLSLE_ADD_TPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLE_LDST8_TPREL_LO12] = "R_AARCH64_TLSLE_LDST8_TPREL_LO12";
  reloc_name[R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC] = "R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLE_LDST16_TPREL_LO12] = "R_AARCH64_TLSLE_LDST16_TPREL_LO12";
  reloc_name[R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC] = "R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLE_LDST32_TPREL_LO12] = "R_AARCH64_TLSLE_LDST32_TPREL_LO12";
  reloc_name[R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC] = "R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLE_LDST64_TPREL_LO12] = "R_AARCH64_TLSLE_LDST64_TPREL_LO12";
  reloc_name[R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC] = "R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSDESC_LD_PREL19] = "R_AARCH64_TLSDESC_LD_PREL19";
  reloc_name[R_AARCH64_TLSDESC_ADR_PREL21] = "R_AARCH64_TLSDESC_ADR_PREL21";
  reloc_name[R_AARCH64_TLSDESC_ADR_PAGE21] = "R_AARCH64_TLSDESC_ADR_PAGE21";
  reloc_name[R_AARCH64_TLSDESC_LD64_LO12] = "R_AARCH64_TLSDESC_LD64_LO12";
  reloc_name[R_AARCH64_TLSDESC_ADD_LO12] = "R_AARCH64_TLSDESC_ADD_LO12";
  reloc_name[R_AARCH64_TLSDESC_OFF_G1] = "R_AARCH64_TLSDESC_OFF_G1";
  reloc_name[R_AARCH64_TLSDESC_OFF_G0_NC] = "R_AARCH64_TLSDESC_OFF_G0_NC";
  reloc_name[R_AARCH64_TLSDESC_LDR] = "R_AARCH64_TLSDESC_LDR";
  reloc_name[R_AARCH64_TLSDESC_ADD] = "R_AARCH64_TLSDESC_ADD";
  reloc_name[R_AARCH64_TLSDESC_CALL] = "R_AARCH64_TLSDESC_CALL";
  reloc_name[R_AARCH64_TLSLE_LDST128_TPREL_LO12] = "R_AARCH64_TLSLE_LDST128_TPREL_LO12";
  reloc_name[R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC] = "R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC";
  reloc_name[R_AARCH64_TLSLD_LDST128_DTPREL_LO12] = "R_AARCH64_TLSLD_LDST128_DTPREL_LO12";
  reloc_name[R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC] = "R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC";
  reloc_name[R_AARCH64_COPY] = "R_AARCH64_COPY";
  reloc_name[R_AARCH64_GLOB_DAT] = "R_AARCH64_GLOB_DAT";
  reloc_name[R_AARCH64_JUMP_SLOT] = "R_AARCH64_JUMP_SLOT";
  reloc_name[R_AARCH64_RELATIVE] = "R_AARCH64_RELATIVE";
  reloc_name[R_AARCH64_TLS_DTPMOD64] = "R_AARCH64_TLS_DTPMOD64";
  reloc_name[R_AARCH64_TLS_DTPREL64] = "R_AARCH64_TLS_DTPREL64";
  reloc_name[R_AARCH64_TLS_TPREL64] = "R_AARCH64_TLS_TPREL64";
  reloc_name[R_AARCH64_TLS_DTPMOD] = "R_AARCH64_TLS_DTPMOD";
  reloc_name[R_AARCH64_TLS_DTPREL] = "R_AARCH64_TLS_DTPREL";
  reloc_name[R_AARCH64_TLS_TPREL] = "R_AARCH64_TLS_TPREL";
  reloc_name[R_AARCH64_TLSDESC] = "R_AARCH64_TLSDESC";
  reloc_name[R_AARCH64_IRELATIVE] = "R_AARCH64_IRELATIV";
}

#define SHF_WRITE	(1 << 0)
#define SHF_ALLOC	(1 << 1)
#define SHF_EXECINSTR	(1 << 2)
#define SHF_MERGE	(1 << 4)
#define SHF_STRINGS	(1 << 5)
#define SHF_INFO_LINK	(1 << 6)
#define SHF_LINK_ORDER	(1 << 7)
#define SHF_OS_NONCONFORMING (1 << 8)
#define SHF_GROUP	(1 << 9)
#define SHF_TLS		(1 << 10)
#define SHF_COMPRESSED	(1 << 11)
#define SHF_MASKOS	0x0FF00000
#define SHF_GNU_BUILD_NOTE    (1 << 20)
#define SHF_MASKPROC	0xF0000000
#define SHF_EXCLUDE	0x80000000
#define SHF_GNU_MBIND	0x01000000

#define SHT_NULL	  0
#define SHT_PROGBITS	  1
#define SHT_SYMTAB	  2
#define SHT_STRTAB	  3
#define SHT_RELA	  4
#define SHT_HASH	  5
#define SHT_DYNAMIC	  6
#define SHT_NOTE	  7
#define SHT_NOBITS	  8
#define SHT_REL		  9
#define SHT_SHLIB	  10
#define SHT_DYNSYM	  11
#define SHT_INIT_ARRAY	  14
#define SHT_FINI_ARRAY	  15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP	  17
#define SHT_SYMTAB_SHNDX  18
#define SHT_LOOS	0x60000000
#define SHT_HIOS	0x6fffffff
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7FFFFFFF
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xFFFFFFFF

#define STN_UNDEF	0

#define STB_LOCAL	0
#define STB_GLOBAL	1
#define STB_WEAK	2
#define STB_LOOS	10
#define STB_GNU_UNIQUE	10
#define STB_HIOS	12
#define STB_LOPROC	13
#define STB_HIPROC	15

#define STT_NOTYPE	0
#define STT_OBJECT	1
#define STT_FUNC	2
#define STT_SECTION	3
#define STT_FILE	4
#define STT_COMMON	5
#define STT_TLS		6
#define STT_RELC	8
#define STT_SRELC	9
#define STT_LOOS	10
#define STT_GNU_IFUNC	10
#define STT_HIOS	12
#define STT_LOPROC	13
#define STT_HIPROC	15

#define GET_QI(x)     *(unsigned char *)&(x)
#define GET_HI(x)     *(unsigned short *)&(x)
#define GET_SI(x)     *(unsigned int *)&(x)
#define GET_DI(x)     *(unsigned long long *)&(x)
#define SET_QI(x, y)  *(unsigned char *)&(x) = y;
#define SET_HI(x, y)  *(unsigned short *)&(x) = y;
#define SET_SI(x, y)  *(unsigned int *)&(x) = y;
#define SET_DI(x, y)  *(unsigned long long *)&(x) = y;

#define EI_NIDENT 16
#define FREAD_CHECK(BUF, SIZE, NUM, STREAM) \
        do \
        { \
          ret = fread(BUF, SIZE, NUM, STREAM); \
          if (ret != NUM) \
            return -EIO; \
        } \
        while (0)

typedef struct
{
  unsigned char	e_ident[EI_NIDENT];     /* ELF "magic number" */
  unsigned char	e_type[2];		/* Identifies object file type */
  unsigned char	e_machine[2];		/* Specifies required architecture */
  unsigned char	e_version[4];		/* Identifies object file version */
  unsigned char	e_entry[8];		/* Entry point virtual address */
  unsigned char	e_phoff[8];		/* Program header table file offset */
  unsigned char	e_shoff[8];		/* Section header table file offset */
  unsigned char	e_flags[4];		/* Processor-specific flags */
  unsigned char	e_ehsize[2];		/* ELF header size in bytes */
  unsigned char	e_phentsize[2];		/* Program header table entry size */
  unsigned char	e_phnum[2];		/* Program header table entry count */
  unsigned char	e_shentsize[2];		/* Section header table entry size */
  unsigned char	e_shnum[2];		/* Section header table entry count */
  unsigned char	e_shstrndx[2];		/* Section header string table index */
} Elf64Hdr;

typedef struct
{
  unsigned char	sh_name[4];
  unsigned char	sh_type[4];
  unsigned char	sh_flags[8];
  unsigned char	sh_addr[8];
  unsigned char	sh_offset[8];
  unsigned char	sh_size[8];
  unsigned char	sh_link[4];
  unsigned char	sh_info[4];
  unsigned char	sh_addralign[8];
  unsigned char	sh_entsize[8];
} Elf64Shdr;

typedef struct {
  unsigned char r_offset[8];
  unsigned char	r_info[8];
  unsigned char	r_addend[8];
} Elf64Rela;

#define ELF64_R_SYM(i)          ((i) >> 32)
#define ELF64_R_TYPE(i)         ((i) & 0xffffffff)
#define ELF64_R_INFO(s,t)       (((uint64_t) (s) << 31 << 1) + (uint64_t) (t))

typedef struct {
  unsigned char	st_name[4];
  unsigned char	st_info[1];
  unsigned char	st_other[1];
  unsigned char	st_shndx[2];
  unsigned char	st_value[8];
  unsigned char	st_size[8];
} Elf64Sym;

#define ELF64_ST_BIND(info)          (((uint32_t)(info)) >> 4)
#define ELF64_ST_TYPE(info)          ((info) & 0xf)
#define ELF64_ST_INFO(bind, type)    (((bind) << 4) + ((type) & 0xf))

#define PG(x)        ((x) & ~(uint64_t)0xfff)
#define PG_OFFSET(x) ((x) &  (uint64_t)0xfff)

#define MASK(n) ((1u << (n)) - 1)

static void MCC_CleanupLocalStackRef_NaiveRCFast()
{
}

static void MCC_DecRef_NaiveRCFast()
{
}

static void MCC_GetOrInsertLiteral()
{
}

static void MCC_IncRef_NaiveRCFast()
{
}

static void **sec_addr;

static void relocate_sec(Elf64Rela *rel, int num, void *sec, Elf64Sym *symtab,
                         char *strtab)
{
  for (int idx = 0; idx < num; idx++) {
    int64_t addend = (int64_t)(GET_DI(rel[idx].r_addend));
    int symidx = ELF64_R_SYM(GET_DI(rel[idx].r_info));
    int64_t rel_off = GET_DI(rel[idx].r_offset);
    Elf64Sym *sym = symtab + symidx;
    int symtype = ELF64_ST_TYPE(GET_QI(sym->st_info));
    int reltype = ELF64_R_TYPE(GET_DI(rel[idx].r_info));
    bool is_64bit_reloc = false, nop_call = false;
    uint32_t cand_4byte;
    uint64_t cand_8byte;
    int64_t relval = 0;

    if (symtype == STT_SECTION) {
      relval = (uint64_t)sec_addr[GET_HI(sym->st_shndx)];
    } else {
      const char *symname = strtab + GET_SI(sym->st_name);
      if (!strcmp(symname, "puts")) {
        relval = (uint64_t)puts;
      } else if (!strcmp(symname, "MCC_IncRef_NaiveRCFast")) {
        relval = (uint64_t)MCC_IncRef_NaiveRCFast;
        nop_call = true;
      } else if (!strcmp(symname, "MCC_DecRef_NaiveRCFast")) {
        relval = (uint64_t)MCC_DecRef_NaiveRCFast;
        nop_call = true;
      } else if (!strcmp(symname, "MCC_CleanupLocalStackRef_NaiveRCFast")) {
        relval = (uint64_t)MCC_CleanupLocalStackRef_NaiveRCFast;
        nop_call = true;
      } else if (!strcmp(symname, "MCC_GetOrInsertLiteral")) {
        relval = (uint64_t)MCC_GetOrInsertLiteral;
        nop_call = true;
      }
    }

    relval += addend;
    cand_4byte = *(uint32_t *)(sec + rel_off);
    cand_8byte = *(uint64_t *)(sec + rel_off);

    switch (reltype) {
    case R_AARCH64_ADR_PREL_PG_HI21:
      relval = PG(relval) - PG((int64_t)sec);
      relval >>= 12;
      cand_4byte = (cand_4byte & ~((MASK (2) << 29) | (MASK (19) << 5)))
                    | ((relval & MASK (2)) << 29)
                    | ((relval & (MASK (19) << 2)) << 3);

      break;
    case R_AARCH64_ADD_ABS_LO12_NC:
      relval = PG_OFFSET(relval);
      cand_4byte = ((cand_4byte & ~(MASK (12) << 10))
                    | ((relval & MASK (12)) << 10));
      break;
    case R_AARCH64_LDST64_ABS_LO12_NC:
      relval = PG_OFFSET(relval);
      relval >>= 3;
      cand_4byte = ((cand_4byte & ~(MASK (12) << 10))
                    | ((relval & MASK (12)) << 10));
      break;
    case R_AARCH64_CALL26:
      if (nop_call) {
        /* mov x0, x0. */
        cand_4byte = 0xaa0003e0;
        break;
      }
      relval = relval - ((int64_t)sec + rel_off);
      relval >>= 2;
      cand_4byte &= ~MASK(26);
      cand_4byte |= relval & MASK(26);
      break;
    case R_AARCH64_PREL32:
      relval = relval - ((int64_t)sec + rel_off);
      cand_4byte = relval;
      break;
    case R_AARCH64_PREL64:
      relval = relval - ((int64_t)sec + rel_off);
      cand_8byte = relval;
      is_64bit_reloc = true;
      break;
    case R_AARCH64_ABS32:
      cand_4byte = relval;
      break;
    case R_AARCH64_ABS64:
      cand_8byte = relval;
      is_64bit_reloc = true;
      break;
    default:
      printf("Unsupported relocation type: %s\n", reloc_name[reltype]);
      continue;
    }

    if (is_64bit_reloc)
      *(uint64_t *)(sec + rel_off) = cand_8byte;
    else
      *(uint32_t *)(sec + rel_off) = cand_4byte;
  }
}

static void *get_addr_from_sym(const char *sym_name, Elf64Shdr *shdr,
                               int e_shnum)
{
  const Elf64Sym *symtab;
  const char *strtab;
  int i, j, cnt;

  for (i = 0; i < e_shnum; i++) {
    uint32_t sec_type = GET_SI(shdr[i].sh_type);

    if (sec_type != SHT_SYMTAB)
      continue;

    symtab = sec_addr[i];
    cnt = GET_DI(shdr[i].sh_size) / GET_DI(shdr[i].sh_entsize);
    strtab = sec_addr[GET_SI(shdr[i].sh_link)];
    for (j = 0; j < cnt; j++) {
      const Elf64Sym *sym = symtab + j;

      if (strcmp(strtab + GET_SI(sym->st_name), sym_name))
        continue;

      return sec_addr[GET_HI(sym->st_shndx)] + GET_DI(sym->st_value);
    }
  }
}

/* ARC has internal string representation. */
static int local_java_sys_printf(void *object, void *str)
{
  int len = *(unsigned char *)(str + 8 + 4);
  char *buf;

  len = (len - 1) / 2;
  buf = calloc(1, len);
  if (!buf)
    return -ENOMEM;
  strncpy(buf, str + 20, len);
  printf("%s", buf);
  free(buf);
  fflush(stdout);
  return 0;
}

static int special_section_hack(Elf64Shdr *shdr, int e_shnum)
{
  void **data_addr = get_addr_from_sym("_PTR__cinf_Ljava_2Flang_2FSystem_3B",
                                       shdr, e_shnum);
  void *fake_data = malloc(64);

  if (!fake_data)
    return -ENOMEM;

  *data_addr = fake_data;
  *(void **)(fake_data + 48) = fake_data + 56;

  data_addr = get_addr_from_sym("_PTRLjava_2Flang_2FSystem_3B_7Cout",
                                shdr, e_shnum);
  /* Alloc independent fake data. */
  fake_data = malloc(256);
  if (!fake_data)
    return -ENOMEM;

  *data_addr = fake_data;
  *(void **)(fake_data + 0) = fake_data + 56;
  *(void **)(fake_data + 56) = fake_data + 64;

  void *fake_data2 = malloc(512);
  if (!fake_data2)
    return -ENOMEM;
  *(void **)(fake_data + 64 + 24) = fake_data2;
  *(void **)(fake_data2 + 312) = local_java_sys_printf;

  return 0;
}

int main(int argc, char *argv[]) {
  uint16_t e_shentsize, e_shnum, e_shstrndx;
  uint64_t sec_size, e_shoff, dummy_val;
  int verbose_level, ret, idx;
  Elf64Shdr *shdr, *str_shdr;
  unsigned char *shstrtab;
  Elf64Hdr elf_hdr;
  FILE *fp;

  if (argc != 4) {
    fprintf(stderr, "Usage: pacific arc_obj entry_sec verbose_level\n");
    fprintf(stderr, "       entry_sec: ELF section index or entry function symbol\n");
    fprintf(stderr, "       verbose_level: bigger for more verbose info, 0 for no verbose info\n");
    return 1;
  }

  verbose_level = atoi(argv[3]);
  init_reloc_name();

  fp = fopen(argv[1], "r");
  if (!fp)
    return -EIO;

  /* ELF header */
  FREAD_CHECK(&elf_hdr, sizeof(elf_hdr), 1, fp);

  /* Section headers. */
  e_shoff = GET_DI(elf_hdr.e_shoff);
  e_shentsize = GET_HI(elf_hdr.e_shentsize);
  e_shnum = GET_HI(elf_hdr.e_shnum);
  e_shstrndx = GET_HI(elf_hdr.e_shstrndx);
  fseek(fp, e_shoff, SEEK_SET);
  shdr = malloc(e_shnum * e_shentsize);
  if (!shdr)
    return -ENOMEM;
  FREAD_CHECK(shdr, sizeof(*shdr), e_shnum, fp);

  sec_addr = calloc(e_shnum, sizeof(void *));
  if (!sec_addr)
    return -ENOMEM;

  /* Section header string table. */
  str_shdr = shdr + e_shstrndx;
  fseek(fp, GET_DI(str_shdr->sh_offset), SEEK_SET);
  sec_size = GET_DI(str_shdr->sh_size);
  shstrtab = malloc(sec_size);
  if (!shstrtab)
    return -ENOMEM;
  /* String table. */
  FREAD_CHECK(shstrtab, sec_size, 1, fp);

  for (idx = 0; idx < e_shnum; idx++) {
    uint32_t sec_type = GET_SI(shdr[idx].sh_type);
    uint64_t sec_flag = GET_DI(shdr[idx].sh_flags);

    if (!((sec_flag & SHF_ALLOC)
        ||sec_type == SHT_PROGBITS
        || sec_type == SHT_RELA
        || sec_type == SHT_SYMTAB
        || sec_type == SHT_STRTAB))
      continue;;

    sec_size = GET_DI(shdr[idx].sh_size);
    /* Sec like .bss. */
    if (sec_type == SHT_NOBITS) {
      sec_addr[idx] = calloc(1, sec_size);
      continue;
    } else if (!sec_size) {
      continue;
    }

    if (verbose_level)
      printf("sec loading: %s\n", shstrtab + GET_SI(shdr[idx].sh_name));

    fseek(fp, GET_DI(shdr[idx].sh_offset), SEEK_SET);
    /* For executable section, remap it.
     * TBD: mmap from FD directly, but may need to map some pages twice.
     */
    if (sec_flag & SHF_EXECINSTR) {
      sec_addr[idx] = mmap(NULL, sec_size, PROT_WRITE | PROT_EXEC,
                           MAP_ANON | MAP_PRIVATE, -1, 0);
      if (sec_addr[idx] == (void *)-1)
        return -errno;
    } else {
      sec_addr[idx] = malloc(sec_size);
      if (!sec_addr[idx])
        return -ENOMEM;
    }
    FREAD_CHECK(sec_addr[idx], sec_size, 1, fp);
  }


  for (idx = 0; idx < e_shnum; idx++) {
    uint32_t sec_type = GET_SI(shdr[idx].sh_type);
    uint32_t sh_link, sh_info;

    if (sec_type != SHT_RELA)
      continue;

    sh_link = GET_SI(shdr[idx].sh_link);
    sh_info = GET_SI(shdr[idx].sh_info);

    if (verbose_level)
      printf("sec relocating: %s\n", shstrtab + GET_SI(shdr[idx].sh_name));

    relocate_sec(sec_addr[idx],
                 GET_DI(shdr[idx].sh_size) / GET_DI(shdr[idx].sh_entsize),
                 sec_addr[sh_info], sec_addr[sh_link],
                 sec_addr[GET_SI(shdr[sh_link].sh_link)]);
  }


  ret = special_section_hack(shdr, e_shnum);
  if (ret < 0)
    return ret;

  /* Assume either section index or function symbol and utilize section 0 is
   * NULL section which could not be entry section.
   */
  int entry_sec_idx = atoi(argv[2]);
  void (*func)();

  if (!entry_sec_idx)
    func = get_addr_from_sym(argv[2], shdr, e_shnum);
  else
    func = sec_addr[entry_sec_idx];

  if (verbose_level)
    printf("entry addr: 0x%p\n", func);

  void *old_sp = NULL, *new_stack = malloc(1024 * 1024);
  if (!new_stack)
    return -ENOMEM;

  /* ARK code-gen seems to be using x19 as a special register for
   * "yieldpoint" check, fake it at the moment.
   *
   * Create independent stack for loaded program.
   */
  __asm__("mov %0, sp\n"
          "mov sp, %1\n"
          "mov x19, %2"
          :"=r"(old_sp)
          :"r"(new_stack), "r"(&dummy_val)
          :);

  func();

  /* Restore sp. */
  __asm__("mov sp, %0"
          :
          :"r"(old_sp)
          :);

  fclose(fp);
  return 0;
}
