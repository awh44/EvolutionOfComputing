#Connect the constant output to accumulators 16-19
p c.o 1
#Need to shift the upper digits right by 5
p 1 ad.s.1.-5
p ad.s.1.-5 3
p 1 a16.alpha
p 3 a17.alpha
p 1 a18.alpha
p 3 a19.alpha
#Connect output of a19 to beta input of a18
p a19.A 2
p 2 a18.beta
#Connect output of a18 to beta input of a17
p a18.A 2
p 2 a17.beta
#Connect output of a17 to beta input of a16
p a17.A 2
p 2 a16.beta

#Initiating unit triggers first constant and a16
p i.io 1-1
p 1-1 c.26i
p 1-1 a16.5i
p a16.5o 1-2
#a17 and second constant triggered by a16
p 1-2 c.27i
p 1-2 a17.5i
p a17.5o 1-3
#a18 and third constant triggered by a17
p 1-3 c.28i
p 1-3 a18.5i
p a18.5o 1-4
#a19 and third constant triggered by a18
p 1-4 c.29i
p 1-4 a19.5i
p a19.5o 1-5
#a19 triggers the master programmer to start the loop
p 1-5 p.Ci
#If continuing, trigger the next cycle
p p.C1o 1-6
#Master programmer triggers adding a17 to a16
p 1-6 a17.7i
p 1-6 a16.6i
p a16.6o 1-7
#a16 triggers adding a18 to a17
p 1-7 a18.7i
p 1-7 a17.6i
p a17.6o 1-8
#a17 triggers adding a19 to a18
p 1-8 a19.7i
p 1-8 a18.6i
p a18.6o 1-9
#a18 triggers printing when done
p 1-9 i.pi
#Trigger back to the master programmer when done printing
p i.po 1-5

#For the accumulators:
#	op5 is for the alpha inputs (used for initial constants)
#	op6 is for the beta inputs (used in the additions)
#	op7 is the positive output (used in the additions)
s a16.op5 alpha
s a16.op6 beta
s a17.op5 alpha
s a17.op6 beta
s a17.op7 A
s a18.op5 alpha
s a18.op6 beta
s a18.op7 A
s a19.op5 alpha
s a19.op7 A

#Do ten iterations on the master programmer.
s p.d17s1 0
s p.d16s1 0
s p.d15s1 1
s p.d14s1 0
s p.d14s2 1
s p.cC 4

#The y value is in j1-j5
s c.s26 Jl
s c.j1 5
s c.j2 4
s c.j3 0
s c.j4 0
s c.j5 0
#The y1 value is in j6-j10
s c.s27 Jr
s c.j6 6
s c.j7 0
s c.j8 0
s c.j9 0
s c.j10 0
#The y2 value is in k1-k5
s c.s28 Kl
s c.k1 2
s c.k2 1
s c.k3 0
s c.k4 0
s c.k5 0
#The y3 value is in k6-k10
s c.s29 Kr
s c.k6 6
s c.k7 0
s c.k8 0
s c.k9 0
s c.k10 0

#Print a16
s pr.7 P
s pr.8 P
