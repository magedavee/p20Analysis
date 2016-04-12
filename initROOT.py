import os
import ROOT
from ROOT import gROOT

def initROOT():
    print 'Init ROOT'
    gROOT.ProcessLine(".L /home/mage/Analysis/RawData-build/libRawData.so")
    gROOT.ProcessLine(".L /home/mage/Analysis/RawData/include/RawData.h")
    gROOT.ProcessLine(".L /home/mage/Analysis/RawData-build/libPmtData.so")
    gROOT.ProcessLine(".L /home/mage/Analysis/RawData/include/PmtData.h")
