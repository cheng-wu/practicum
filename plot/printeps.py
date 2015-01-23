#!/usr/bin/python

import os, sys, tempfile

# Perform some checks
if len(sys.argv) != 2:
    print 'Usage: printeps.py inputfolder \n'
    print 'Example: printeps graphs \n'
    sys.exit(-1)

inputdir = sys.argv[1]
if not os.path.isdir(inputdir):
    print '%s is NOT directory or does not exist' % inputdir
    sys.exit(-1)

# Prepare the latex file.
foundeps = False
tmpdir = tempfile.mkdtemp(dir='.')
tex = open('%s/all.tex' % tmpdir, 'w')
tex.write('\\documentclass[10pt]{article}\n')
tex.write('\\usepackage{epsfig}\n')
tex.write('\\begin{document}\n\n')
for (dp, dn, filenames) in os.walk(os.path.abspath(inputdir)):
    for f in filenames:
        if f.endswith('.eps'):
            foundeps = True
            tex.write('\\begin{figure}\n')
            tex.write('\\includegraphics[width=\\textwidth]{%s/%s}\n' % (dp, f))
            tex.write('\\end{figure}\n\n')
tex.write('\\end{document}\n')
tex.close()

# Generate pdf file and clean up.
if not foundeps:
    print 'No eps files found'
    sys.exit(-1)

curdir = os.path.abspath('.')
os.chdir(tmpdir)
os.system('latex all')
os.system('dvips -o all.ps all')
os.system('ps2pdf14 all.ps')
os.system('cp all.pdf ../')
os.chdir(curdir)
os.system('rm -rf %s' % tmpdir)