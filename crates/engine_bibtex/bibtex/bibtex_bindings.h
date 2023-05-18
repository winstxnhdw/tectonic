#ifndef BIBTEX_BINDINGS_H
#define BIBTEX_BINDINGS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "tectonic_bridge_core.h"

#define HASH_BASE 1

typedef enum {
  BUF_TY_BASE,
  BUF_TY_SV,
  BUF_TY_EX,
  BUF_TY_OUT,
  BUF_TY_NAME_SEP,
} BufTy;

typedef enum {
  CResult_Error,
  CResult_Recover,
  CResult_Ok,
} CResult;

typedef enum {
  FN_CLASS_BUILTIN = 0,
  FN_CLASS_WIZARD = 1,
  FN_CLASS_INT_LIT = 2,
  FN_CLASS_STR_LIT = 3,
  FN_CLASS_FIELD = 4,
  FN_CLASS_INT_ENTRY_VAR = 5,
  FN_CLASS_STR_ENTRY_VAR = 6,
  FN_CLASS_INT_GLBL_VAR = 7,
  FN_CLASS_STR_GLBL_VAR = 8,
} FnClass;

typedef enum {
  HISTORY_SPOTLESS = 0,
  HISTORY_WARNING_ISSUED = 1,
  HISTORY_ERROR_ISSUED = 2,
  HISTORY_FATAL_ERROR = 3,
  HISTORY_ABORTED = 4,
} History;

/**
 * The lexer class of a character - this represents whether the parser considers it to be alphabetic,
 * numeric, etc. Illegal represents tokens that shouldn't show up at all, such as ASCII backspace.
 *
 */
typedef enum {
  LEX_CLASS_ILLEGAL = 0,
  LEX_CLASS_WHITESPACE = 1,
  LEX_CLASS_ALPHA = 2,
  LEX_CLASS_NUMERIC = 3,
  LEX_CLASS_SEP = 4,
  LEX_CLASS_OTHER = 5,
} LexClass;

typedef enum {
  SCAN_RES_ID_NULL = 0,
  SCAN_RES_SPECIFIED_CHAR_ADJACENT = 1,
  SCAN_RES_OTHER_CHAR_ADJACENT = 2,
  SCAN_RES_WHITESPACE_ADJACENT = 3,
} ScanRes;

typedef enum {
  STK_TYPE_INTEGER = 0,
  STK_TYPE_STRING = 1,
  STK_TYPE_FUNCTION = 2,
  STK_TYPE_MISSING = 3,
  STK_TYPE_ILLEGAL = 4,
} StkType;

typedef int32_t StrNumber;

typedef int32_t BufPointer;

typedef uint8_t ASCIICode;

typedef struct {
  ASCIICode *name_of_file;
  int32_t name_length;
} NameAndLen;

typedef struct {
  int min_crossrefs;
} BibtexConfig;

typedef ASCIICode *BufType;

typedef int32_t CiteNumber;

typedef int32_t HashPointer2;

typedef struct {
  StkType typ;
  int32_t lit;
} ExecVal;

typedef struct {
  ttbc_input_handle_t *handle;
  int peek_char;
  bool saw_eof;
} PeekableInput;

typedef struct {
  PeekableInput *bst_file;
  StrNumber bst_str;
  int32_t bst_line_num;
  int32_t num_bib_files;
  int32_t num_preamble_strings;
} BstCtx;

typedef struct {
  BstCtx *bst_ctx;
  ExecVal pop1;
  ExecVal pop2;
  ExecVal pop3;
  ExecVal *lit_stack;
  int32_t lit_stk_size;
  int32_t lit_stk_ptr;
  bool mess_with_entries;
  StrNumber bib_str_ptr;
} ExecCtx;

typedef int32_t HashPointer;

typedef int32_t FieldLoc;

typedef uintptr_t PoolPointer;

typedef enum {
  CResultStr_Error,
  CResultStr_Recover,
  CResultStr_Ok,
} CResultStr_Tag;

typedef struct {
  CResultStr_Tag tag;
  union {
    struct {
      StrNumber ok;
    };
  };
} CResultStr;

typedef struct {
  /**
   * The location of the string - where it exists, was inserted, of if insert is false,
   * where it *would* have been inserted
   */
  int32_t loc;
  /**
   * Whether the string existed in the hash table already
   */
  bool exists;
} LookupRes;

typedef enum {
  CResultLookup_Error,
  CResultLookup_Ok,
} CResultLookup_Tag;

typedef struct {
  CResultLookup_Tag tag;
  union {
    struct {
      LookupRes ok;
    };
  };
} CResultLookup;

typedef uint8_t StrIlk;

typedef int32_t AuxNumber;

typedef int32_t BibNumber;

typedef int32_t WizFnLoc;

typedef int32_t FnDefLoc;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern const LexClass LEX_CLASS[256];

extern const int32_t CHAR_WIDTH[256];

void reset_all(void);

bool bib_str_eq_buf(StrNumber s, BufTy buf, BufPointer ptr, BufPointer len);

NameAndLen start_name(StrNumber file_name);

void add_extension(NameAndLen *nal, StrNumber ext);

extern History tt_engine_bibtex_main(ttbc_state_t *api,
                                     const BibtexConfig *cfg,
                                     const char *aux_name);

int32_t bib_buf_size(void);

BufType bib_buf(BufTy ty);

ASCIICode bib_buf_at(BufTy ty, BufPointer num);

ASCIICode bib_buf_at_offset(BufTy ty, uintptr_t num);

BufPointer bib_buf_offset(BufTy ty, uintptr_t num);

void bib_set_buf_offset(BufTy ty, uintptr_t num, BufPointer offset);

BufPointer bib_buf_len(BufTy ty);

void bib_set_buf_len(BufTy ty, BufPointer len);

void buffer_overflow(void);

BufPointer name_tok(BufPointer pos);

void set_name_tok(BufPointer pos, BufPointer val);

void lower_case(BufTy buf, BufPointer ptr, BufPointer len);

void upper_case(BufTy buf, BufPointer ptr, BufPointer len);

BufPointer int_to_ascii(int32_t the_int, BufTy int_buf, BufPointer int_begin);

void quick_sort(CiteNumber left_end, CiteNumber right_end);

StrNumber cite_list(CiteNumber num);

void set_cite_list(CiteNumber num, StrNumber str);

CiteNumber cite_ptr(void);

void set_cite_ptr(CiteNumber num);

void check_cite_overflow(CiteNumber last_cite);

uintptr_t max_cites(void);

StrNumber cite_info(CiteNumber num);

void set_cite_info(CiteNumber num, StrNumber info);

HashPointer2 type_list(CiteNumber num);

void set_type_list(CiteNumber num, HashPointer2 ty);

bool entry_exists(CiteNumber num);

void set_entry_exists(CiteNumber num, bool exists);

bool print_lit(ExecVal val);

bool print_stk_lit(ExecVal val);

bool print_wrong_stk_lit(ExecCtx *ctx, ExecVal val, StkType typ2);

bool bst_ex_warn_print(const ExecCtx *ctx);

bool bst_ln_num_print(const BstCtx *bst_ctx);

bool print_bst_name(const BstCtx *bst_ctx);

void push_lit_stk(ExecCtx *ctx, ExecVal val);

bool pop_lit_stk(ExecCtx *ctx, ExecVal *out);

History get_history(void);

void mark_warning(void);

void mark_error(void);

uint32_t err_count(void);

ttbc_output_handle_t *init_log_file(const char *file);

ttbc_output_handle_t *standard_output(void);

ttbc_output_handle_t *bib_log_file(void);

void putc_log(int c);

void puts_log(const char *str);

void ttstub_puts(ttbc_output_handle_t *handle, const char *s);

void print_overflow(void);

void print_confusion(void);

void print_a_token(void);

bool print_a_pool_str(StrNumber s);

void sam_wrong_file_name_print(NameAndLen file);

bool print_aux_name(void);

bool log_pr_aux_name(void);

bool aux_err_print(void);

bool aux_err_illegal_another_print(int32_t cmd_num);

void aux_err_no_right_brace_print(void);

void aux_err_stuff_after_right_brace_print(void);

void aux_err_white_space_in_argument_print(void);

void aux_end1_err_print(void);

bool aux_end2_err_print(void);

bool print_bib_name(void);

bool log_pr_bib_name(void);

bool log_pr_bst_name(const BstCtx *ctx);

void hash_cite_confusion(void);

bool bst_warn_print(const BstCtx *ctx);

void eat_bst_print(void);

void unknwn_function_class_confusion(void);

bool bst_id_print(ScanRes scan_result);

void bst_left_brace_print(void);

void bst_right_brace_print(void);

bool bib_err_print(bool at_bib_command);

bool bib_warn_print(void);

bool eat_bib_print(bool at_bib_command);

bool bib_one_of_two_print(ASCIICode char1, ASCIICode char2, bool at_bib_command);

bool bib_equals_sign_print(bool at_bib_command);

bool bib_unbalanced_braces_print(bool at_bib_command);

void macro_warn_print(void);

bool bib_id_print(ScanRes scan_res);

void bib_cmd_confusion(void);

void cite_key_disappeared_confusion(void);

bool bad_cross_reference_print(StrNumber s);

bool print_missing_entry(StrNumber s);

bool bst_cant_mess_with_entries_print(const ExecCtx *ctx);

void bst_1print_string_size_exceeded(void);

bool bst_2print_string_size_exceeded(const ExecCtx *ctx);

bool braces_unbalanced_complaint(const ExecCtx *ctx, StrNumber pop_lit_var);

void case_conversion_confusion(void);

void print_fn_class(HashPointer fn_loc);

CResult bst_err_print_and_look_for_blank_line(BstCtx *ctx);

CResult already_seen_function_print(BstCtx *ctx, HashPointer seen_fn_loc);

bool nonexistent_cross_reference_error(FieldLoc field_ptr);

PeekableInput *peekable_open(const char *path, ttbc_file_format format);

int peekable_close(PeekableInput *peekable);

bool tectonic_eof(PeekableInput *peekable);

bool input_ln(PeekableInput *peekable);

bool bib_str_eq_str(StrNumber s1, StrNumber s2);

void pool_overflow(void);

ASCIICode bib_str_pool(PoolPointer idx);

void bib_set_str_pool(PoolPointer idx, ASCIICode code);

StrNumber bib_str_ptr(void);

void bib_set_str_ptr(StrNumber ptr);

PoolPointer bib_str_start(StrNumber s);

void bib_set_str_start(StrNumber s, PoolPointer ptr);

uintptr_t bib_pool_size(void);

uintptr_t bib_max_strings(void);

PoolPointer bib_pool_ptr(void);

void bib_set_pool_ptr(PoolPointer ptr);

CResultStr bib_make_string(void);

CResultLookup str_lookup(BufTy buf, BufPointer ptr, BufPointer len, StrIlk ilk, bool insert);

bool scan1(ASCIICode char1);

bool scan1_white(ASCIICode char1);

bool scan2(ASCIICode char1, ASCIICode char2);

bool scan2_white(ASCIICode char1, ASCIICode char2);

bool scan3(ASCIICode char1, ASCIICode char2, ASCIICode char3);

bool scan_alpha(void);

bool scan_white_space(void);

ScanRes scan_identifier(ASCIICode char1, ASCIICode char2, ASCIICode char3);

bool scan_nonneg_integer(void);

bool scan_integer(int32_t *token_value);

StrNumber cur_aux(void);

void set_cur_aux(StrNumber num);

PeekableInput *cur_aux_file(void);

void set_cur_aux_file(PeekableInput *file);

int32_t cur_aux_ln(void);

void set_cur_aux_ln(int32_t ln);

AuxNumber aux_ptr(void);

void set_aux_ptr(AuxNumber num);

StrNumber cur_bib(void);

void set_cur_bib(StrNumber num);

PeekableInput *cur_bib_file(void);

void set_cur_bib_file(PeekableInput *input);

BibNumber bib_ptr(void);

void set_bib_ptr(BibNumber num);

void check_bib_files(BibNumber ptr);

void add_preamble(StrNumber num);

StrNumber cur_preamble(void);

BibNumber preamble_ptr(void);

void set_preamble_ptr(BibNumber num);

int32_t bib_line_num(void);

void set_bib_line_num(int32_t num);

void reset_after_compute(void);

int32_t end_of_def(void);

int32_t undefined(void);

FnClass fn_type(HashPointer pos);

void set_fn_type(HashPointer pos, FnClass ty);

StrNumber hash_text(HashPointer pos);

void set_hash_text(HashPointer pos, StrNumber num);

int32_t ilk_info(HashPointer pos);

void set_ilk_info(HashPointer pos, int32_t val);

HashPointer hash_next(HashPointer pos);

void set_hash_next(HashPointer pos, HashPointer val);

int32_t hash_size(void);

int32_t hash_prime(void);

void set_hash_prime(int32_t val);

HashPointer2 wiz_functions(WizFnLoc pos);

void set_wiz_functions(WizFnLoc pos, HashPointer2 val);

WizFnLoc wiz_def_ptr(void);

void set_wiz_def_ptr(WizFnLoc val);

void check_grow_wiz(FnDefLoc ptr);

StrNumber field_info(FieldLoc pos);

void set_field_info(FieldLoc pos, StrNumber val);

void check_field_overflow(int32_t total_fields);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* BIBTEX_BINDINGS_H */
