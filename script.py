from ROOT import *
import sys

# call e.g.: python script.py 4 dtc file,file,file,file...

if not len(sys.argv) == 4:
    print("error, please supply channel mask, detector name and files seperated by commas")
else:
    try:
        channelMask = int(sys.argv[1])
        detector  = sys.argv[2]
        files = sys.argv[3]

        from ROOT import *
        gSystem.Load("mergescript.cc")
        gROOT.ProcessLine(".L mergescript.cc")
        gROOT.ProcessLine("mergescript(%s, \"%s\", \"%s\")" % (channelMask, detector, files))
    except Exception as e:
        print(e)
