#include <string.h>
#ifndef FIELD_RULES_H
#define FIELD_RULES_H

typedef struct
{
    char *type;
    char *attributes[10];
    int attr_count;
} FieldRule;

static FieldRule field_rules[] = {
    {"text", {"required", "pattern", "default"}, 3},
    {"textarea", {"rows", "cols", "default"}, 3},
    {"number", {"min", "max", "required"}, 3},
    {"email", {"pattern", "required"}, 2},
    {"date", {"min", "max", "required"}, 3},
    {"checkbox", {"default"}, 1},
    {"dropdown", {"required", "default"}, 2},
    {"radio", {"required"}, 1},
    {"password", {"required", "pattern"}, 2},
    {"file", {"accept", "required"}, 2}};

static int field_rule_count = sizeof(field_rules) / sizeof(FieldRule);

static int is_attribute_valid_for_type(const char *type, const char *attr)
{
    for (int i = 0; i < field_rule_count; i++)
    {
        if (strcmp(field_rules[i].type, type) == 0)
        {
            for (int j = 0; j < field_rules[i].attr_count; j++)
            {
                if (strcmp(field_rules[i].attributes[j], attr) == 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

#endif
