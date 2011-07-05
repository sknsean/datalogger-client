/*
 * Energinet Datalogger
 * Copyright (C) 2009 - 2011 LIAB ApS <info@liab.dk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "uni_data.h"
#include "module_util.h"

#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

/**
 * @ingroup uni_data
 * Data type strings 
 */
const char *datatyps[] = { "int", "float", "flow", "state", "text", "fault",
		NULL };

struct uni_data *uni_data_create(enum data_types type, unsigned long value,
		void* data, unsigned long ms) {
	struct uni_data *new = malloc(sizeof(*new));
	if (!new) {
		PRINT_ERR("malloc failed");
		return NULL;
	}

	new->value = value;
	new->data = data;
	new->type = type;
	new->mtime = ms;
	new->inheap = 1;

	return new;
}

struct uni_data *uni_data_copy(struct uni_data *source) {
	struct uni_data *new = NULL;
	int data_size = 0;
	if (!source)
		return NULL;

	new = malloc(sizeof(*new));
	assert(new);
	memset(new, 0, sizeof(*new));

	new->value = source->value;
	new->type = source->type;
	new->mtime = source->mtime;

	switch (source->type) {
	case DATA_INT:
		data_size = 0;
		break;
	case DATA_FLOAT:
	case DATA_FLOW:
		data_size = sizeof(float);
		break;
	case DATA_STATE:
	case DATA_TEXT:
	case DATA_FAULT:
		if (source->data)
			data_size = strlen(source->data) + 1/* null termination */;
		break;
	default:
		data_size = 0;
		break;
	}

	if (data_size) {
		new->data = malloc(data_size);
		assert(new->data);
		memcpy(new->data, source->data, data_size);
	}

	new->inheap = 1;

	return new;
}

struct uni_data *uni_data_create_int(int value) {
	return uni_data_create(DATA_INT, (unsigned long) value, NULL, 0);
}

struct uni_data *uni_data_create_float(float value) {
	float *valuep = malloc(sizeof(float));

	*valuep = value;

	return uni_data_create(DATA_FLOAT, (float) value, valuep, 0);
}

struct uni_data *uni_data_create_flow(float count, unsigned long ms) {
	float *valuep = malloc(sizeof(float));
	*valuep = count;
	return uni_data_create(DATA_FLOW, (float) count, valuep, ms);
}

struct uni_data *uni_data_create_text(const char *text) {
	if (text)
		return uni_data_create(DATA_TEXT, 0, strdup(text), 0);
	else
		return uni_data_create(DATA_TEXT, 0, strdup("NULL"), 0);
}

struct uni_data *uni_data_create_state(unsigned long state, const char *text) {
	if (text)
		return uni_data_create(DATA_STATE, state, strdup(text), 0);
	else
		return uni_data_create(DATA_STATE, state, NULL, 0);
}

struct uni_data *uni_data_create_from_str(enum data_types type, const char *str) {

	switch (type) {
	case DATA_INT: {
		int value = 0;
		sscanf(str, "%d", &value);
		return uni_data_create_int(value);
	}
	case DATA_FLOAT: {
		float value = 0;
		sscanf(str, "%f", &value);
		return uni_data_create_float(value);
	}
	case DATA_FLOW: {
		float count = 0;
		unsigned long ms = 0;
		sscanf(str, "%f:%lu", &count, &ms);
		return uni_data_create_flow(count, ms);
	}

	case DATA_TEXT: {
		return uni_data_create_text(str);
	}
	case DATA_FAULT:
	case DATA_STATE:
		fprintf(stderr, "uni_data_create_from_str type %s not implemented\n",
				datatyps[type]);
		break;
	default:
		fprintf(stderr, "uni_data_create_from_str type %s not implemented\n",
				"unknown!");
		break;
	}

	return NULL;

}

void uni_data_delete(struct uni_data *rem) {

	if (!rem->inheap)
		return;

	if (rem->data)
		free(rem->data);

	free(rem);
}

int uni_data_get_txt_value(struct uni_data *data, char* buffer, int max_size) {
	int ptr = 0;
	switch (data->type) {
	case DATA_INT:
		ptr += snprintf(buffer + ptr, max_size - ptr, "%d", data->value);
		break;
	case DATA_FLOAT:
	case DATA_FLOW: {
		float *value = (float*) data->data;
		ptr += snprintf(buffer + ptr, max_size - ptr, "%f", *value);
	}
		break;
	case DATA_TEXT:
		ptr += snprintf(buffer + ptr, max_size - ptr, "%s", (char*) data->data);
		break;
	case DATA_STATE:
		ptr += snprintf(buffer + ptr, max_size - ptr, "%d", data->value);
		if (data->data)
			ptr += snprintf(buffer + ptr, max_size - ptr, ": %s",
					(char*) data->data);

		break;
	case DATA_FAULT:
		ptr += snprintf(buffer + ptr, max_size - ptr, "unknown");
		break;
	default:
		ptr += snprintf(buffer + ptr, max_size - ptr, "unknown");
		break;
	}

	return ptr;

}

int uni_data_get_txt_fvalue(struct uni_data *data, char* buffer, int max_size,
		struct ftolunit *ftlunit) {
	int ptr = 0;
	float lvalue = 0;
	switch (data->type) {
	case DATA_INT:
		lvalue = module_calc_get_level(0, data->value, ftlunit);
		ptr += snprintf(buffer + ptr, max_size - ptr, "%.0f", lvalue);
		break;
	case DATA_FLOAT:
	case DATA_FLOW: {
		float *value = (float*) data->data;
		char fstr[100];
		int decs = 2;
		if (ftlunit)
			decs = ftlunit->decs;
		snprintf(fstr, sizeof(fstr), "%%.%df", decs);
		lvalue = module_calc_get_level(data->mtime, *value, ftlunit);
		ptr += snprintf(buffer + ptr, max_size - ptr, fstr, lvalue);
	}
		break;
	case DATA_TEXT:
		ptr += snprintf(buffer + ptr, max_size - ptr, "%s", (char*) data->data);
		break;
	case DATA_STATE:
		ptr += snprintf(buffer + ptr, max_size - ptr, "%d", data->value);
		if (data->data)
			ptr += snprintf(buffer + ptr, max_size - ptr, ": %s",
					(char*) data->data);

		break;
	case DATA_FAULT:
		ptr += snprintf(buffer + ptr, max_size - ptr, "unknown");
		break;
	default:
		ptr += snprintf(buffer + ptr, max_size - ptr, "unknown");
		break;
	}

	return ptr;

}

float uni_data_get_fvalue(struct uni_data *data) {
	float value = 0;

	switch (data->type) {
	case DATA_STATE:
	case DATA_INT:
		value = data->value;
		break;
	case DATA_FLOAT: {
		float *valuep = (float*) data->data;
		value = *valuep;
		break;
	}
	case DATA_FLOW: {
		float *valuep = (float*) data->data;
		value = *valuep;
		if (data->mtime)
			value /= (((float) data->mtime) / 1000);

		printf("flow %f/%f = %f\n", *valuep, (((float) data->mtime) / 1000),
				value);
		break;
	}
	case DATA_TEXT:
	default:
		break;
	}
	return value;
}

float uni_data_get_value(struct uni_data *data) {
	float value = 0;

	switch (data->type) {
	case DATA_STATE:
	case DATA_INT:
		value = data->value;
		break;
	case DATA_FLOW:
	case DATA_FLOAT: {
		float *valuep = (float*) data->data;
		value = *valuep;
		break;
	}
	case DATA_TEXT:
	default:
		break;
	}
	return value;
}

enum data_types uni_data_type_str(const char *text) {

	return (enum data_types) modutil_get_listitem(text, 0, datatyps);
}

const char * uni_data_str_type(enum data_types type) {
	return datatyps[type];
}

