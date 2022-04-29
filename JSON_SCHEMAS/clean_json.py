
import sys

def clean_json(path):
    ''' '''
    fp = open(path, "r")
    text = fp.read()
    fp.close()
    
    def remove_comments(text):
        lines = text.split("\n")
        xlines = []
        # remove comments
        for line in lines:
            try:
                a = line.split("#", 1)[0]
            except Exception, _:
                a = line
            xlines.append(a.rstrip())
        return "\n".join(xlines)
    
    def remove_last_trailing_comma(text):
        lines = text.split("\n")
        xlines = []
        # remove "last" trailing comma in objects
        for k, line in enumerate(lines):
            xline = line.rstrip()
            if xline.endswith(","):
                #print "xxxx: <%s> : <%s>" % ( xline, lines[k+1])
                next_line = lines[k+1].lstrip()
                if next_line.startswith("}"):
                    xline = xline[:-1]
                else:
                    xline = xline
            else:
                xline = xline
            xlines.append(xline)
        return "\n".join(xlines)
        
    text = remove_comments(text)
    text = remove_last_trailing_comma(text)
    
    return text
    
def main(path):
    print clean_json(path)
 
    
if __name__ == '__main__':
    main(sys.argv[1])
