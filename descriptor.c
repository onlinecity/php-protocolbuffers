#include "php_protocol_buffers.h"
#include "descriptor.h"

static const char *fields_map[] = {
	"DUMMY",
	"TYPE_DOUBLE",
	"TYPE_FLOAT",
	"TYPE_INT64",
	"TYPE_UINT64",
	"TYPE_INT32",
	"TYPE_FIXED64",
	"TYPE_FIXED32",
	"TYPE_BOOL",
	"TYPE_STRING",
	"TYPE_GROUP",
	"TYPE_MESSAGE",
	"TYPE_BYTES",
	"TYPE_UINT32",
	"TYPE_ENUM",
	"TYPE_SFIXED32",
	"TYPE_SFIXED64",
	"TYPE_SINT32",
	"TYPE_SINT64",
};

int php_pb_descriptor_properties_init(zval *object TSRMLS_DC)
{
	zval *pp = NULL;
	HashTable *properties = NULL;

	ALLOC_HASHTABLE(properties);
	zend_hash_init(properties, 0, NULL, ZVAL_PTR_DTOR, 0);

	MAKE_STD_ZVAL(pp);
	array_init(pp);
	zend_hash_update(properties, "fields", sizeof("fields"), (void **)&pp, sizeof(zval), NULL);

	zend_merge_properties(object, properties, 1 TSRMLS_CC);
	return 0;
}


static const char* field_type_to_str(int field_type)
{
	if (field_type > 0 && field_type <= MAX_FIELD_TYPE) {
		return fields_map[field_type];
	} else {
		return NULL;
	}
}

static void php_protocolbuffers_descriptor_free_storage(php_protocolbuffers_descriptor *object TSRMLS_DC)
{
	if (object->name_len > 0) {
		efree(object->name);
	}

	if (object->container != NULL && object->free_container < 1) {
		int i;

		for (i = 0; i < (object->container)->size; i++) {
			if ((object->container)->scheme[i].original_name != NULL) {
				efree((object->container)->scheme[i].original_name);
			}
			if ((object->container)->scheme[i].name != NULL) {
				efree((object->container)->scheme[i].name);
			}
			if ((object->container)->scheme[i].mangled_name != NULL) {
				efree((object->container)->scheme[i].mangled_name);
			}
		}

		if (object->container->single_property_name != NULL) {
			efree(object->container->single_property_name);
		}

		if (object->container->scheme != NULL) {
			efree(object->container->scheme);
		}

		if (object->container->extensions != NULL) {
			efree(object->container->extensions);
		}

		efree(object->container);
	}

	zend_object_std_dtor(&object->zo TSRMLS_CC);
	efree(object);
}

zend_object_value php_protocolbuffers_descriptor_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	PHP_PROTOCOLBUFFERS_STD_CREATE_OBJECT(php_protocolbuffers_descriptor);

	object->name = NULL;
	object->name_len = 0;
	object->free_container = 0;
	object->container = (pb_scheme_container*)emalloc(sizeof(pb_scheme_container));
	pb_scheme_container_init(object->container);

	return retval;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_pb_descriptor___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pb_descriptor_get_name, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pb_descriptor_get_field, 0, 0, 1)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pb_descriptor_get_fields, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pb_descriptor_get_option, 0, 0, 1)
	ZEND_ARG_INFO(0, option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pb_descriptor_dump, 0, 0, 0)
ZEND_END_ARG_INFO()


/* {{{ proto ProtocolBuffersDescriptor ProtocolBuffersDescriptor::__construct()
*/
PHP_METHOD(protocolbuffers_descriptor, __construct)
{
	/* do not create instance by user. we expect it always created from builder. */
}
/* }}} */

/* {{{ proto string ProtocolBuffersDescriptor::getName()
*/
PHP_METHOD(protocolbuffers_descriptor, getName)
{
	zval *instance = getThis();
	php_protocolbuffers_descriptor *descriptor;

	descriptor = PHP_PROTOCOLBUFFERS_GET_OBJECT(php_protocolbuffers_descriptor, instance);

	RETURN_STRINGL(descriptor->name, descriptor->name_len, 1);
}
/* }}} */

/* {{{ proto ProtocolBuffersFieldDescriptor ProtocolBuffersDescriptor::getField(int $tag)
*/
PHP_METHOD(protocolbuffers_descriptor, getField)
{
	zval *instance = getThis();
	zval **result;
	char *property;
	int property_len;
	long tag = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"l", &tag) == FAILURE) {
		return;
	}

	zend_mangle_property_name(&property, &property_len, (char*)"*", 1, "fields", sizeof("fields"), 0);
	if (zend_hash_find(Z_OBJPROP_P(instance), property, property_len, (void **)&result) == SUCCESS) {
		zval **entry;
		if (zend_hash_index_find(Z_ARRVAL_PP(result), tag, (void **)&entry) == SUCCESS) {
			RETVAL_ZVAL(*entry, 0, 1);
		}
	}
	efree(property);
}
/* }}} */

/* {{{ proto ProtocolBuffersFieldDescriptor ProtocolBuffersDescriptor::getFields()
*/
PHP_METHOD(protocolbuffers_descriptor, getFields)
{
	zval *instance = getThis();
	zval **result;
	char *property;
	int property_len;

	zend_mangle_property_name(&property, &property_len, (char*)"*", 1, "fields", sizeof("fields"), 0);
	if (zend_hash_find(Z_OBJPROP_P(instance), property, property_len, (void **)&result) == SUCCESS) {
		RETVAL_ZVAL(*result, 0, 1);
	}
	efree(property);
}
/* }}} */

/* {{{ proto mixed ProtocolBuffersDescriptor::getOption(int $option)
*/
PHP_METHOD(protocolbuffers_descriptor, getOption)
{
	zend_throw_exception_ex(spl_ce_RuntimeException, 0 TSRMLS_CC, "ProtocolBuffersDescriptor::getOption does not implement yet");
}
/* }}} */

/* {{{ proto void ProtocolBuffersDescriptor::dump()
*/
PHP_METHOD(protocolbuffers_descriptor, dump)
{
	zval *instance = getThis();
	php_protocolbuffers_descriptor *descriptor;
	int n = 0;
	pb_scheme *ischeme;


	descriptor = PHP_PROTOCOLBUFFERS_GET_OBJECT(php_protocolbuffers_descriptor, instance);

	php_printf("{\n");
	if (descriptor->name_len > 0) {
		php_printf("  \"name\": \"%s\",\n", descriptor->name);
	} else {
		php_printf("  \"name\": \"\",\n");
	}

	php_printf("  \"fields\": {\n");
	if (descriptor->container->size > 0) {
		for (n = 0; n < descriptor->container->size; n++) {
			ischeme = &(descriptor->container->scheme[n]);
			php_printf("    \"%d\": {\n", ischeme->tag);
			php_printf("      type: \"%s\",\n", field_type_to_str(ischeme->type));
			php_printf("      name: \"%s\",\n", ischeme->name);
			if (ischeme->type == TYPE_MESSAGE && ischeme->ce != NULL) {
				php_printf("      message: \"%s\",\n", ischeme->ce->name);
			}
			php_printf("      repeated: %s,\n", (ischeme->repeated == 1) ? "true" : "false");
			php_printf("      packed: %s\n", (ischeme->packed == 1) ? "true" : "false");
			php_printf("    }");

			if (n+1 < descriptor->container->size) {
				php_printf(",\n");
			} else {
				php_printf("\n");
			}

		}
	}
	php_printf("  }\n");

	php_printf("  \"extension_ranges\": {\n");
	if (descriptor->container->extension_cnt > 0) {
		pb_extension_range *range;
		for (n = 0; n < descriptor->container->extension_cnt; n++) {
			range = &(descriptor->container->extensions[n]);

			php_printf("    \"%d\": {\n", n);
			php_printf("      begin: %d,\n", range->begin);
			php_printf("      end: %d,\n", range->end);
			php_printf("    }");

			if (n+1 < descriptor->container->extension_cnt) {
				php_printf(",\n");
			} else {
				php_printf("\n");
			}
		}
	}
	php_printf("  }\n");

	php_printf("}\n");
}
/* }}} */

static zend_function_entry php_protocolbuffers_descriptor_methods[] = {
	PHP_ME(protocolbuffers_descriptor, __construct, arginfo_pb_descriptor___construct, ZEND_ACC_PRIVATE)
	PHP_ME(protocolbuffers_descriptor, getName,     arginfo_pb_descriptor_get_name,    ZEND_ACC_PUBLIC)
	PHP_ME(protocolbuffers_descriptor, getField,    arginfo_pb_descriptor_get_field,   ZEND_ACC_PUBLIC)
	PHP_ME(protocolbuffers_descriptor, getFields,   arginfo_pb_descriptor_get_fields,  ZEND_ACC_PUBLIC)
	PHP_ME(protocolbuffers_descriptor, getOption,   arginfo_pb_descriptor_get_option,  ZEND_ACC_PUBLIC)
	PHP_ME(protocolbuffers_descriptor, dump,        arginfo_pb_descriptor_dump,        ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void php_pb_descriptor_class(TSRMLS_D)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "ProtocolBuffersDescriptor", php_protocolbuffers_descriptor_methods);
	protocol_buffers_descriptor_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	protocol_buffers_descriptor_class_entry->create_object = php_protocolbuffers_descriptor_new;

	zend_declare_property_null(protocol_buffers_descriptor_class_entry, "fields", sizeof("fields")-1, ZEND_ACC_PROTECTED TSRMLS_CC);

	PHP_PROTOCOLBUFFERS_REGISTER_NS_CLASS_ALIAS(PHP_PROTOCOLBUFFERS_NAMESPACE, "Descriptor", protocol_buffers_descriptor_class_entry);
}
