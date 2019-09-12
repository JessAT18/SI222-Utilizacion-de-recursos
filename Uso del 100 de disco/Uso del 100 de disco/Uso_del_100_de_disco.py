def get_filenames():
    for (dirpath, dirnames, filenames) in os.walk("articles/"):
        dirs.extend(dirnames)

    for dir in dirs:
        path = "Jessica" + "\\" + dir        
        nxml_files.extend(glob.glob(path + "/*.nxml"))

    return nxml_files

def extract_text_from_files(nxml_files):  
    for nxml_file in nxml_files:       
        fast_parse(nxml_file)

def fast_parse(infile):
    file = open(infile,"r")
    filetext = file.read()
    tag_breaks = filetext.split('><')
    paragraphs = [tag_break.strip('p>').strip('</') for tag_break in tag_breaks if tag_break.startswith('p>')]

def run_files(): 
    nxml_files = get_filenames()
    extract_text_from_files(nxml_files)

if _name_ == "_main_":    
    run_files()
