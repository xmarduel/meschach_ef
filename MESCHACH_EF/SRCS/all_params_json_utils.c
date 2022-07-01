/** \file all_params_json_utils.c
 *
 * misc functions to read a json config file containing comments!
 *
 *
 */

#include <string.h>
#include <stdio.h>

#include "MESCHACH_EF/INCLUDES/all_params_json_utils.h"

#define BUFSIZE 65536
#define LINSIZE 1024
#define CMDSIZE 1024

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

static char *get_file_text_remove_all_comments(const char *path)
{
   char *buffer = calloc(BUFSIZE, sizeof(char));
   char line[LINSIZE];
   char cmd[CMDSIZE];

   int  pos = 0;

   snprintf(cmd, CMDSIZE, "awk 'BEGIN { FS = \"#\" } {print $1}' %s", path);

   FILE *stream;

   if ((stream = popen(cmd, "r")) == NULL)
   {
      perror("get_file_text_remove_all_comments: popen() failed");
      return NULL;
   }

   while ( fgets(line, LINSIZE, stream) != NULL && pos < BUFSIZE )
   {
      //printf("got %s", line);
      strcpy(buffer + pos, line);
      pos += strlen(line);
   }

   pclose(stream);

   if ( pos > BUFSIZE )
   {
      printf("get_file_text_remove_all_comments: file too big for buffer of size %d", BUFSIZE);
      return NULL;
   }

   return buffer;
}

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

static void json_dump_error(const char* prefix, json_error_t *error)
{
   fprintf(stderr, "%s\n", prefix);
   fprintf(stderr, "\t error: text %s\n", error->text);
   fprintf(stderr, "\t error: source %s\n", error->source);
   fprintf(stderr, "\t error: line %d\n", error->line);
   fprintf(stderr, "\t error: position %d\n", error->position);
}

json_t *json_load_xfile(const char *path)
{
   int k;
   char *text = get_file_text_remove_all_comments(path);
   json_t *root;
   json_error_t error;

   root = json_loads(text, 0, &error);

   if(!root)
   {
      json_dump_error("json_load_xfile", &error);
      return NULL;
   }

   return root;
}

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

int json_object_get_boolean_value(const json_t *p, const char *objname)
{
   json_t* o = json_object_get(p, objname);

   if ( o == NULL )
   {
   }

   return json_boolean_value(o);
}

const char *json_object_get_string_value(const json_t *p, const char *objname)
{
   json_t* o = json_object_get(p, objname);

   if ( o == NULL )
   {
   }

   return json_string_value(o);
}

int json_object_get_integer_value(const json_t *p, const char *objname)
{
   json_t* o = json_object_get(p, objname);

   if ( o == NULL )
   {
   }

   return json_integer_value(o);
}

double json_object_get_real_value(const json_t *p, const char *objname)
{
   json_t* o = json_object_get(p, objname);

   if ( o == NULL )
   {
   }

   return json_real_value(o);
}

/* ---------------------------------------------------------------------- */

int json_array_get_integer_value(const json_t *p, int idx)
{
   json_t* o = json_array_get(p, idx);

   if ( o == NULL )
   {
   }

   return json_integer_value(o);
}

double json_array_get_real_value(const json_t *p, int idx)
{
   json_t* o = json_array_get(p, idx);

   if ( o == NULL )
   {
   }

   return json_real_value(o);
}

const char* json_array_get_string_value(const json_t *p, int idx)
{
   json_t* o = json_array_get(p, idx);

   if ( o == NULL )
   {
   }

   return json_string_value(o);
}

/* ---------------------------------------------------------------------- */

int json_object_get_arrayitem_integer_value(const json_t *p, const char *objname, int idx)
{
   json_t* o = json_object_get(p, objname);

   if ( o == NULL )
   {
   }

   return json_array_get_integer_value(o, idx);
}

double json_object_get_arrayitem_real_value(const json_t *p, const char *objname, int idx)
{
   json_t* o = json_object_get(p, objname);

   if ( o == NULL )
   {
   }

   return json_array_get_real_value(o, idx);
}

const char* json_object_get_arrayitem_string_value(const json_t *p, const char *objname, int idx)
{
   json_t* o = json_object_get(p, objname);

   if ( o == NULL )
   {
   }

   return json_array_get_string_value(o, idx);
}

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

/* json validation */

int json_check_data(const char* path, const char* schema, char *res)
{
   int status;
   char *text = get_file_text_remove_all_comments(path);

   if ( text == NULL )
   {
      return 1;
   }

   FILE *tmp = fopen("schema_without_comments.json", "w");

   fprintf(tmp, "%s", text);
   fclose(tmp);

   char cmd[CMDSIZE];
   char PYTHON_EXE[64] = "/Library/Frameworks/Python.framework/Versions/2.7/bin/python";
   char CHECKER[99]    = "JSON_SCHEMAS/check_json.py";
   char JSON_FILE[32]  = "schema_without_comments.json";

   char checker_abs_path[512];
   Params_get_absolute_path(CHECKER, checker_abs_path);

   snprintf(cmd, CMDSIZE, "%s %s %s %s", PYTHON_EXE, checker_abs_path, JSON_FILE, schema);

   FILE *stream;
   char line[1024];

   if ((stream = popen(cmd, "r")) == NULL)
   {
      perror("json_check_data: popen() failed");
      return 1;
   }

   while(fgets(line, LINSIZE, stream) != NULL)
   {
      printf("%s", line);
   }
   status = pclose(stream);

   printf("json_check_data returned %d. \n", status);

   return status;
}

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */

