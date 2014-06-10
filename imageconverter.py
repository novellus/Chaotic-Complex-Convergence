FileName='saved_fb'
#This will convert the output file format of the chaotic mapping program into a pbm image file format.

inpt=open(FileName,"rb")
outpt=open(FileName+'.pbm','w')
x=1680*2
a=inpt.read()
y=len(a)/x

outpt.write('''P1
'''+str(x/2)+''' '''+str(y))

for _y in range(y):
    outpt.write('''
''')
    for _x in range(0,x-2,2):
        if a[_x+x*_y]=='\xff':
            outpt.write('0 ')
        else:
            outpt.write('1 ')
    if a[x-2+x*_y]=='\xff':
        outpt.write('0')
    else:
        outpt.write('1')

inpt.close()
outpt.close()
