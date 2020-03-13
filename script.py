from ROOT import *
import sys

# call e.g.: python script.py 4 dtc 

if not len(sys.argv) == 3:
    print("error, please supply channel mask and detector name")
else:
    try:
        channelMask = int(sys.argv[1])
        detector  = sys.argv[2]

        from ROOT import *
        gSystem.Load("mergescript.cc")
        gROOT.ProcessLine(".L mergescript.cc")
        gROOT.ProcessLine("mergescript(%s, \"%s\")" % (channelMask, detector))
    except Exception as e:
        print(e)
