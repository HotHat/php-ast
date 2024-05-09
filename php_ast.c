/* php_ast extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_types.h"
#include "ext/standard/info.h"
#include "php_php_ast.h"
#include "php_ast_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

#define DEBUG 1
#if defined(DEBUG)
#define AST_DEBUG(s, c, i) php_printf(s, c, i);
#else
#define AST_DEBUG(s, c, i)
#endif

static void parse(zend_ast *ast, HashTable *ht);

static zval ast_kind_to_string(int kind)
{
	zval str;
    switch (kind)
    {
    case ZEND_AST_ZVAL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ZVAL");
        break;
    case ZEND_AST_CONSTANT:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CONSTANT");
        break;
    case ZEND_AST_ZNODE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ZNODE");
        break;
    case ZEND_AST_FUNC_DECL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_FUNC_DECL");
        break;
    case ZEND_AST_CLOSURE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CLOSURE");
        break;
    case ZEND_AST_METHOD:
        ZVAL_STRING_FAST(&str, "ZEND_AST_METHOD");
        break;
    case ZEND_AST_CLASS:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CLASS");
        break;
    case ZEND_AST_ARROW_FUNC:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ARROW_FUNC");
        break;
    case ZEND_AST_ARG_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ARG_LIST");
        break;
    case ZEND_AST_ARRAY:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ARRAY");
        break;
    case ZEND_AST_ENCAPS_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ENCAPS_LIST");
        break;
    case ZEND_AST_EXPR_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_EXPR_LIST");
        break;
    case ZEND_AST_STMT_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_STMT_LIST");
        break;
    case ZEND_AST_IF:
        ZVAL_STRING_FAST(&str, "ZEND_AST_IF");
        break;
    case ZEND_AST_SWITCH_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_SWITCH_LIST");
        break;
    case ZEND_AST_CATCH_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CATCH_LIST");
        break;
    case ZEND_AST_PARAM_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PARAM_LIST");
        break;
    case ZEND_AST_CLOSURE_USES:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CLOSURE_USES");
        break;
    case ZEND_AST_PROP_DECL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PROP_DECL");
        break;
    case ZEND_AST_CONST_DECL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CONST_DECL");
        break;
    case ZEND_AST_CLASS_CONST_DECL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CLASS_CONST_DECL");
        break;
    case ZEND_AST_NAME_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_NAME_LIST");
        break;
    case ZEND_AST_TRAIT_ADAPTATIONS:
        ZVAL_STRING_FAST(&str, "ZEND_AST_TRAIT_ADAPTATIONS");
        break;
    case ZEND_AST_USE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_USE");
        break;
    case ZEND_AST_TYPE_UNION:
        ZVAL_STRING_FAST(&str, "ZEND_AST_TYPE_UNION");
        break;
    case ZEND_AST_TYPE_INTERSECTION:
        ZVAL_STRING_FAST(&str, "ZEND_AST_TYPE_INTERSECTION");
        break;
    case ZEND_AST_ATTRIBUTE_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ATTRIBUTE_LIST");
        break;
    case ZEND_AST_ATTRIBUTE_GROUP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ATTRIBUTE_GROUP");
        break;
    case ZEND_AST_MATCH_ARM_LIST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_MATCH_ARM_LIST");
        break;
    case ZEND_AST_MAGIC_CONST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_MAGIC_CONST");
        break;
    case ZEND_AST_TYPE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_TYPE");
        break;
    case ZEND_AST_CONSTANT_CLASS:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CONSTANT_CLASS");
        break;
    case ZEND_AST_CALLABLE_CONVERT:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CALLABLE_CONVERT");
        break;
    case ZEND_AST_VAR:
        ZVAL_STRING_FAST(&str, "ZEND_AST_VAR");
        break;
    case ZEND_AST_CONST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CONST");
        break;
    case ZEND_AST_UNPACK:
        ZVAL_STRING_FAST(&str, "ZEND_AST_UNPACK");
        break;
    case ZEND_AST_UNARY_PLUS:
        ZVAL_STRING_FAST(&str, "ZEND_AST_UNARY_PLUS");
        break;
    case ZEND_AST_UNARY_MINUS:
        ZVAL_STRING_FAST(&str, "ZEND_AST_UNARY_MINUS");
        break;
    case ZEND_AST_CAST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CAST");
        break;
    case ZEND_AST_EMPTY:
        ZVAL_STRING_FAST(&str, "ZEND_AST_EMPTY");
        break;
    case ZEND_AST_ISSET:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ISSET");
        break;
    case ZEND_AST_SILENCE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_SILENCE");
        break;
    case ZEND_AST_SHELL_EXEC:
        ZVAL_STRING_FAST(&str, "ZEND_AST_SHELL_EXEC");
        break;
    case ZEND_AST_CLONE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CLONE");
        break;
    case ZEND_AST_EXIT:
        ZVAL_STRING_FAST(&str, "ZEND_AST_EXIT");
        break;
    case ZEND_AST_PRINT:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PRINT");
        break;
    case ZEND_AST_INCLUDE_OR_EVAL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_INCLUDE_OR_EVAL");
        break;
    case ZEND_AST_UNARY_OP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_UNARY_OP");
        break;
    case ZEND_AST_PRE_INC:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PRE_INC");
        break;
    case ZEND_AST_PRE_DEC:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PRE_DEC");
        break;
    case ZEND_AST_POST_INC:
        ZVAL_STRING_FAST(&str, "ZEND_AST_POST_INC");
        break;
    case ZEND_AST_POST_DEC:
        ZVAL_STRING_FAST(&str, "ZEND_AST_POST_DEC");
        break;
    case ZEND_AST_YIELD_FROM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_YIELD_FROM");
        break;
    case ZEND_AST_CLASS_NAME:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CLASS_NAME");
        break;
    case ZEND_AST_GLOBAL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_GLOBAL");
        break;
    case ZEND_AST_UNSET:
        ZVAL_STRING_FAST(&str, "ZEND_AST_UNSET");
        break;
    case ZEND_AST_RETURN:
        ZVAL_STRING_FAST(&str, "ZEND_AST_RETURN");
        break;
    case ZEND_AST_LABEL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_LABEL");
        break;
    case ZEND_AST_REF:
        ZVAL_STRING_FAST(&str, "ZEND_AST_REF");
        break;
    case ZEND_AST_HALT_COMPILER:
        ZVAL_STRING_FAST(&str, "ZEND_AST_HALT_COMPILER");
        break;
    case ZEND_AST_ECHO:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ECHO");
        break;
    case ZEND_AST_THROW:
        ZVAL_STRING_FAST(&str, "ZEND_AST_THROW");
        break;
    case ZEND_AST_GOTO:
        ZVAL_STRING_FAST(&str, "ZEND_AST_GOTO");
        break;
    case ZEND_AST_BREAK:
        ZVAL_STRING_FAST(&str, "ZEND_AST_BREAK");
        break;
    case ZEND_AST_CONTINUE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CONTINUE");
        break;
    case ZEND_AST_DIM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_DIM");
        break;
    case ZEND_AST_PROP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PROP");
        break;
    case ZEND_AST_NULLSAFE_PROP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_NULLSAFE_PROP");
        break;
    case ZEND_AST_STATIC_PROP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_STATIC_PROP");
        break;
    case ZEND_AST_CALL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CALL");
        break;
    case ZEND_AST_CLASS_CONST:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CLASS_CONST");
        break;
    case ZEND_AST_ASSIGN:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ASSIGN");
        break;
    case ZEND_AST_ASSIGN_REF:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ASSIGN_REF");
        break;
    case ZEND_AST_ASSIGN_OP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ASSIGN_OP");
        break;
    case ZEND_AST_BINARY_OP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_BINARY_OP");
        break;
    case ZEND_AST_GREATER:
        ZVAL_STRING_FAST(&str, "ZEND_AST_GREATER");
        break;
    case ZEND_AST_GREATER_EQUAL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_GREATER_EQUAL");
        break;
    case ZEND_AST_AND:
        ZVAL_STRING_FAST(&str, "ZEND_AST_AND");
        break;
    case ZEND_AST_OR:
        ZVAL_STRING_FAST(&str, "ZEND_AST_OR");
        break;
    case ZEND_AST_ARRAY_ELEM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ARRAY_ELEM");
        break;
    case ZEND_AST_NEW:
        ZVAL_STRING_FAST(&str, "ZEND_AST_NEW");
        break;
    case ZEND_AST_INSTANCEOF:
        ZVAL_STRING_FAST(&str, "ZEND_AST_INSTANCEOF");
        break;
    case ZEND_AST_YIELD:
        ZVAL_STRING_FAST(&str, "ZEND_AST_YIELD");
        break;
    case ZEND_AST_COALESCE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_COALESCE");
        break;
    case ZEND_AST_ASSIGN_COALESCE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ASSIGN_COALESCE");
        break;
    case ZEND_AST_STATIC:
        ZVAL_STRING_FAST(&str, "ZEND_AST_STATIC");
        break;
    case ZEND_AST_WHILE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_WHILE");
        break;
    case ZEND_AST_DO_WHILE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_DO_WHILE");
        break;
    case ZEND_AST_IF_ELEM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_IF_ELEM");
        break;
    case ZEND_AST_SWITCH:
        ZVAL_STRING_FAST(&str, "ZEND_AST_SWITCH");
        break;
    case ZEND_AST_SWITCH_CASE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_SWITCH_CASE");
        break;
    case ZEND_AST_DECLARE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_DECLARE");
        break;
    case ZEND_AST_USE_TRAIT:
        ZVAL_STRING_FAST(&str, "ZEND_AST_USE_TRAIT");
        break;
    case ZEND_AST_TRAIT_PRECEDENCE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_TRAIT_PRECEDENCE");
        break;
    case ZEND_AST_METHOD_REFERENCE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_METHOD_REFERENCE");
        break;
    case ZEND_AST_NAMESPACE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_NAMESPACE");
        break;
    case ZEND_AST_USE_ELEM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_USE_ELEM");
        break;
    case ZEND_AST_TRAIT_ALIAS:
        ZVAL_STRING_FAST(&str, "ZEND_AST_TRAIT_ALIAS");
        break;
    case ZEND_AST_GROUP_USE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_GROUP_USE");
        break;
    case ZEND_AST_CLASS_CONST_GROUP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CLASS_CONST_GROUP");
        break;
    case ZEND_AST_ATTRIBUTE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ATTRIBUTE");
        break;
    case ZEND_AST_MATCH:
        ZVAL_STRING_FAST(&str, "ZEND_AST_MATCH");
        break;
    case ZEND_AST_MATCH_ARM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_MATCH_ARM");
        break;
    case ZEND_AST_NAMED_ARG:
        ZVAL_STRING_FAST(&str, "ZEND_AST_NAMED_ARG");
        break;
    case ZEND_AST_METHOD_CALL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_METHOD_CALL");
        break;
    case ZEND_AST_NULLSAFE_METHOD_CALL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_NULLSAFE_METHOD_CALL");
        break;
    case ZEND_AST_STATIC_CALL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_STATIC_CALL");
        break;
    case ZEND_AST_CONDITIONAL:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CONDITIONAL");
        break;
    case ZEND_AST_TRY:
        ZVAL_STRING_FAST(&str, "ZEND_AST_TRY");
        break;
    case ZEND_AST_CATCH:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CATCH");
        break;
    case ZEND_AST_PROP_GROUP:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PROP_GROUP");
        break;
    case ZEND_AST_PROP_ELEM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PROP_ELEM");
        break;
    case ZEND_AST_CONST_ELEM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CONST_ELEM");
        break;
    case ZEND_AST_CONST_ENUM_INIT:
        ZVAL_STRING_FAST(&str, "ZEND_AST_CONST_ENUM_INIT");
        break;
    case ZEND_AST_FOR:
        ZVAL_STRING_FAST(&str, "ZEND_AST_FOR");
        break;
    case ZEND_AST_FOREACH:
        ZVAL_STRING_FAST(&str, "ZEND_AST_FOREACH");
        break;
    case ZEND_AST_ENUM_CASE:
        ZVAL_STRING_FAST(&str, "ZEND_AST_ENUM_CASE");
        break;
    case ZEND_AST_PARAM:
        ZVAL_STRING_FAST(&str, "ZEND_AST_PARAM");
        break;

    default: {
        char *st = (char *)malloc(40);
        sprintf(st, "UNKNOWN: %d", kind);
        ZVAL_STRING(&str, st);
    }
    }
    return str;
}

static bool zend_ast_kind_is_decl(zend_ast_kind kind)
{
    return kind == ZEND_AST_FUNC_DECL || kind == ZEND_AST_CLOSURE || kind == ZEND_AST_ARROW_FUNC || kind == ZEND_AST_METHOD || kind == ZEND_AST_CLASS;
}

static node_ast *create_node_ast()
{
    node_ast *nast = (node_ast *)malloc(sizeof(node_ast) - sizeof(node_ast *) + sizeof(node_ast *) * 100);
    nast->kind = "UNINITIALIZED";
    nast->value = "";
    nast->lineno = 0;
    nast->children = 0;

    return nast;
}

static void parse_list(zend_ast *ast, HashTable *ht)
{
	zend_string *children = zend_string_init_fast("children", 8);
	zend_string *num_str = zend_string_init_fast("num", 3);
//	zend_string_release(children);
//	zend_string_release(num_str);

    zend_ast_list *ast_list = zend_ast_get_list(ast);
    int num = 0;
    int max = ast_list->children;

    HashTable *ht_children = zend_new_array(max);

	zend_string *kind = zend_string_init_fast("kind", 4);
	zend_string_release(kind);
	zend_string *kd = Z_STR_P(zend_hash_find(ht, kind));
    for (uint32_t i = 0; i < max; i++)
    {
        if (ast_list->child[i])
        {
            AST_DEBUG("list %s [%d]\n", ZSTR_VAL(kd), i);
			HashTable *ht_ch = zend_new_array(0);
//            nast->child[nast->children] = create_node_ast();
            parse(ast_list->child[i], ht_ch);

			zval ch;
			ZVAL_ARR(&ch, ht_ch);
			zend_hash_next_index_insert(ht_children, &ch);
            num++;
        }
        else
        {
            AST_DEBUG("list %s [%d] not found\n", ZSTR_VAL(kd), i);
        }
    }

	// add key: children
    zval zch;
    ZVAL_ARR(&zch, ht_children);
	zend_hash_add(ht, children, &zch);

    // add key: num
	zval znum;
	ZVAL_LONG(&znum, num);
	zend_hash_add(ht, num_str, &znum);
}

static void parse_as_list(zend_ast *ast, HashTable *ht)
{
    int num = 0;
    int max = zend_ast_get_num_children(ast);

    HashTable *ht_children = zend_new_array(max);

	zend_string *kind = zend_string_init_fast("kind", 4);
	zend_string *num_str = zend_string_init_fast("num", 3);
	zend_string *children = zend_string_init_fast("children", 8);

	zend_string *kd = Z_STR_P(zend_hash_find(ht, kind));
    for (uint32_t i = 0; i < max; i++)
    {
        if (ast->child[i])
        {
            AST_DEBUG("as_list %s [%d]\n", ZSTR_VAL(kd), i);

			HashTable *ht_ch = zend_new_array(0);

//            nast->child[nast->children] = create_node_ast();
            parse(ast->child[i], ht_ch);

			zval ch;
			ZVAL_ARR(&ch, ht_ch);
			zend_hash_next_index_insert(ht_children, &ch);

            num++;
        }
        else
        {
            AST_DEBUG("as_list %s [%d] not found\n", ZSTR_VAL(kd), i);
        }
    }

	// add key: children
    zval zch;
    ZVAL_ARR(&zch, ht_children);
	zend_hash_add(ht, children, &zch);

    // add key: num
	zval znum;
	ZVAL_LONG(&znum, num);
	zend_hash_add(ht, num_str, &znum);

	// clean
	zend_string_release(kind);
	zend_string_release(num_str);
	zend_string_release(children);
}

void parse_decl(zend_ast_decl *ast, HashTable *ht)
{
    int num = 0;
    HashTable *ht_children = zend_new_array(0);

	zend_string *kind = zend_string_init_fast("kind", 4);
	zend_string *num_str = zend_string_init_fast("num", 3);
	zend_string *children = zend_string_init_fast("children", 8);

	zend_string *kd = Z_STR_P(zend_hash_find(ht, kind));

    for (uint32_t i = 0; i < 5; i++)
    {
        if (ast->child[i])
        {
            AST_DEBUG("decl %s [%d]\n", ZSTR_VAL(kd), i);
			HashTable *ht_ch = zend_new_array(0);
            parse(ast->child[i], ht_ch);

			zval ch;
			ZVAL_ARR(&ch, ht_ch);
			zend_hash_next_index_insert(ht_children, &ch);
            num++;
        }
        else
        {
            AST_DEBUG("decl %s [%d] not found\n", ZSTR_VAL(kd), i);
        }
    }

    // add key: children
	zval zch;
	ZVAL_ARR(&zch, ht_children);
	zend_hash_add(ht, children, &zch);

	// add key: num
	zval znum;
	ZVAL_LONG(&znum, num);
	zend_hash_add(ht, num_str, &znum);

	// clean
	zend_string_release(kind);
	zend_string_release(num_str);
	zend_string_release(children);
}

void parse(zend_ast *ast, HashTable *ht)
{
	zend_string *kind = zend_string_init_fast("kind", 4);
	zend_string *lineno = zend_string_init_fast("lineno", 6);
	zend_string *value = zend_string_init_fast("value", 5);

    if (ast->kind == ZEND_AST_ZVAL)
    {
		zval kd = ast_kind_to_string(ast->kind);
    	zend_hash_add(ht, kind, &kd);
    	zval no;
    	ZVAL_LONG(&no, zend_ast_get_lineno(ast));
    	zend_hash_add(ht, lineno, &no);

//        nast->kind = ast_kind_to_string(ast->kind);
//        nast->lineno = zend_ast_get_lineno(ast);

        zval *zv = zend_ast_get_zval(ast);

    	zend_hash_add(ht, value, zv);
        goto clean;
    }

    if (zend_ast_kind_is_decl(ast->kind))
    {
        zend_ast_decl *decl = (zend_ast_decl *)ast;

//        nast->kind = ast_kind_to_string(ast->kind);
//        nast->lineno = zend_ast_get_lineno(ast);
//        nast->value = decl->name->val;
        zval kd = ast_kind_to_string(ast->kind);
		zend_hash_add(ht, kind, &kd);

		zval no;
		ZVAL_LONG(&no, zend_ast_get_lineno(ast));
		zend_hash_add(ht, lineno, &no);

        zval str;
        ZVAL_STRING(&str, decl->name->val);
    	zend_hash_add(ht, value, &str);

        parse_decl(decl, ht);
        goto clean;
    }

    if (zend_ast_is_list(ast))
    {
//        nast->kind = ast_kind_to_string(ast->kind);
//        nast->lineno = zend_ast_get_lineno(ast);
		zval kd = ast_kind_to_string(ast->kind);
		zend_hash_add(ht, kind, &kd);

		zval no;
		ZVAL_LONG(&no, zend_ast_get_lineno(ast));
		zend_hash_add(ht, lineno, &no);

		parse_list(ast, ht);
        goto clean;
    }

//    nast->kind = ast_kind_to_string(ast->kind);
//    nast->lineno = zend_ast_get_lineno(ast);
	zval kd = ast_kind_to_string(ast->kind);
	zend_hash_add(ht, kind, &kd);

	zval no;
	ZVAL_LONG(&no, zend_ast_get_lineno(ast));
	zend_hash_add(ht, lineno, &no);
    parse_as_list(ast, ht);

clean:
	zend_string_release(kind);
	zend_string_release(lineno);
	zend_string_release(value);
}

zval *get_zval_by_name(char *name)
{
    HashTable *symbol_table = zend_array_dup(zend_rebuild_symbol_table());

    zend_string *key_name = zend_string_init(name, strlen(name), 0);
    zval *data = zend_hash_find(symbol_table, key_name);

    zend_string_release(key_name);
    zend_array_destroy(symbol_table);

    return data;
}

/* {{{ array get_ast(string $code) */
PHP_FUNCTION(get_ast)
{
	char *source_code;
	size_t code_len;
	zval *retval;
	node_ast *nast = create_node_ast();

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(source_code, code_len)
	ZEND_PARSE_PARAMETERS_END();

	if (!code_len) {
		RETURN_NULL();
	}

	zend_arena *ast_arena;
	zval str_val;
	ZVAL_STRING(&str_val, source_code);
	zend_ast *ast = zend_compile_string_to_ast(Z_STR(str_val), &ast_arena, ZSTR_EMPTY_ALLOC());

	if (!ast)
	{
		RETURN_NULL();
	}



//	zval tmp;
//	zend_ast_ref *ast_ref = zend_ast_copy(ast);
//	ZVAL_AST(&tmp, ast_ref);
//	ZVAL_COPY_VALUE(retval, &tmp);
//	retval = &tmp;
//	zval val;
//    ZVAL_STRING(&val, "foo");
//	ZVAL_NEW_ARR(&val);

	HashTable *ht = zend_new_array(0);
	parse(ast, ht);

//	zend_hash_add(ht, zend_string_init_fast("hello", 5), &val);
	RETURN_ARR(ht);
//	zval val;
//	zend_ast_ref *ast_ref = zend_ast_copy(ast);
//	ZVAL_AST(&val, ast_ref);
//	RETURN_ZVAL(&val, 1, 0);
//	return_value = retval;
}
/* }}}*/

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(php_ast)
{
#if defined(ZTS) && defined(COMPILE_DL_PHP_AST)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(php_ast)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "php_ast support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ php_ast_module_entry */
zend_module_entry php_ast_module_entry = {
	STANDARD_MODULE_HEADER,
	"php_ast",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(php_ast),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(php_ast),			/* PHP_MINFO - Module info */
	PHP_PHP_AST_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PHP_AST
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(php_ast)
#endif
