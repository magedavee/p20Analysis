from initROOT import initROOT
import ROOT
from ROOT import gROOT, TCanvas, TF1,TFile,TTree
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
from scipy.stats import norm 
from array import array
initROOT()
name=[]
# name.append("/home/mage/Data/p20Data/root/P20_2015-04-16-10-44-25.root")
# name.append("/home/mage/Data/p20Data/root/P20_2015-04-16-09-44-16.root")
name.append("/home/mage/Data/p20Data/root/P20_2015-04-16-08-44-07.root")
dtList=[]
integral0=[]
integral1=[]
total=[]
pulseList=[]
for j in xrange(0,1):
    print "processing "+str(name[j])
    pmt=ROOT.PmtData(name[j])
    event=0
    ent= pmt.GetEntries()
    # ent=10000
    pmt.CalIntegral(0)
    pmt.CalIntegral(1)
    for i in xrange(0,ent):
	# print i
	pmt.SetEntry(i)
	i0= pmt.GetPulseIntegral(0,i)
	i1= pmt.GetPulseIntegral(1,i)
	tot=i0+i1
	if pmt.GetNCha() >=4 and tot >140000:
	    i0= pmt.GetPulseIntegral(0,i)
	    i1= pmt.GetPulseIntegral(1,i)
	    tot=i0+i1
	    ch1=pmt.GetPulse(0)
	    ch2=pmt.GetPulse(1)
	    peak1=np.array(ch1).min()
	    peak2=np.array(ch2).min()
	    if  peak1 < - 50 or peak2<-50:
		# pmt.GetTrace(0).Draw()
		# pmt.GetTrace(1).Draw("SAME")
		# input("pause ")
		tr1=pmt.GetTrace(0)
		tr2=pmt.GetTrace(1)
		t1=0
		t2=0
		for i in xrange(1,1100):
		    if tr1.Eval(i) < .5*peak1:
			t1=float(i-1)
			break
		for i in xrange(1,1100):
		    if tr2.Eval(i) < .5*peak2:
			t2=float(i-1)
			break
		dt=t1-t2
		if abs(dt)<50:
		    integral0.append(i0)
		    integral1.append(i1)
		    total.append(float(tot)/20.0)
		    dtList.append(dt)
		    # pulse=np.array(pmt.GetPulse(0))
		    # pulseList.append(pulse)
dt=array('f',[0])
tot=array('f',[0])
left=array('i',[0])
right=array('i',[0])
f=TFile('proData.root','recreate')
tree=TTree('proData','process data')
tree.Branch('dt',dt,'dt/F')
tree.Branch('left',left,'left/I')
tree.Branch('right',right,'right/I')
tree.Branch('tot',tot,'tot/F')
for i in xrange(0,len(dtList)):
    dt[0]=dtList[i]
    left[0]=integral0[i]
    right[0]=integral1[i]
    tot[0]=total[i]
    tree.Fill()
f.Write()
	    # plt.plot(pulse)
	    # plt.show()
# print len(dtList)
# print "mean "+str(np.array(dtList).mean())
# print "std "+ str( np.array(dtList).std())
# mu , sigma=norm.fit(total)
# print "mu "+str(mu)
# print "sigma "+ str(sigma)
# n, bins, patches =plt.hist(np.array(total),bins=50,normed=True)
# n, bins, patches =plt.hist(np.array(dtList),bins=50,normed=True)
# gaus = mlab.normpdf( bins, mu, sigma)
# plt.plot(bins, gaus, 'r--', linewidth=2)
# plt.xlabel("PE")
# plt.show()
# pmt.GetIntegral(0).Draw()
# input()
# pmt.GetIntegral(1).Draw()
# input()
	# pmt.SetEntry(i)
	# print i
	# print pmt.GetNCha()
	# if pmt.GetNCha() >= 4:
	    # pmt.GetTrace(0).Draw()
	    # pmt.GetTrace(1).Draw("SAME")
	    # input("pause ")
# while event >=0:
	# pmt.GetTrace(0).Draw()
	# # input("pause ")
	# pmt.GetTrace(1).SetLineColor(4);
	# pmt.GetTrace(1).Draw("SAME")
	# event=int(raw_input("promt: "))
	# pmt.SetEntry(event)
#############
