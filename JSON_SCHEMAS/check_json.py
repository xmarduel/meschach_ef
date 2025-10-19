import sys
import json

from jsonschema import validate as json_validate
from jsonschema import Draft4Validator

import clean_json


def validate(jsonfile, jsonschema):

    cleaned_json_text = clean_json.clean_json(jsonfile)

    try:
        instance = json.loads(cleaned_json_text)
    except Exception as msg:
        print("FAILED to read file %s : %s" % (jsonfile, str(msg)))
        return -1 
         
    try:
        schema = json.load(open(jsonschema))
    except Exception as msg:
        print("FAILED to read schema file %s : %s" % (jsonschema, str(msg)))
        return -1
        
    try:
        print("")
        print("check schema '%s' Draft4  ..." % jsonschema)
        Draft4Validator.check_schema(schema)
        print("check schema '%s' Draft4Validator OK" % jsonschema)
    except Exception as msg:
        print("check schema '%s' Draft4Validator NOK" % jsonschema)
        print(str(msg))
        return 1
        
    try:
        print("")
        print("validate file '%s' ..." % jsonfile)
        rc = json_validate(instance, schema)
    except Exception as msg:
        print("validation file '%s' NOK" % jsonfile)
        print(str(msg))
        return 1
    
    print("validation file '%s' OK" % jsonfile)
    
    return 0

def main(jsonfile, jsonschema):
    return validate(jsonfile, jsonschema=jsonschema)

def usage():
    print("""
    check_json.py <json file> <json schema>
""")

#
#

if __name__ == '__main__':
    if not len(sys.argv[1:]) == 2:
        usage()
        sys.exit(1)
        
    jsonfile   = sys.argv[1]
    jsonschema = sys.argv[2]

    rc = main(jsonfile, jsonschema)
    
    if rc != 0:
        sys.exit(rc)
        
    sys.exit(0)
