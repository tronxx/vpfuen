#SOLID=/solid2.30/lib/scll2x23.a 
SOLID=/solidSDK30/Linux_glibc2/lib/libsolcli.so
vpg4:	vpg4v.o inpq.o chefun3.o misce.o pan.o impfs.o \
		vsamfun.o vplib.o recup.o vcgi2.o \
			vstrings.o vbsearch.o \
			calcd4.o cmp2.o comma.o vcliser.o \
			getad.o tida.o vblo.o err.o vattr.o vform.o
	gcc -o vpg4 -lform -lm \
    	 vpg4v.o inpq.o chefun3.o misce.o pan.o impfs.o \
			vsamfun.o vplib.o recup.o \
			vstrings.o vbsearch.o vcliser.o \
			calcd4.o cmp2.o comma.o vcgi2.o \
			getad.o tida.o vblo.o err.o vattr.o vform.o \
			-lncurses -lform
	chown root vpg4
	chmod u+s vpg4
	mv vpg4 bin

vpg4s:		vpg4q.o solsq.o getadq.o calcu.o\
		inpq.o chefun3.o misce.o pan.o impfs.o \
		vstrings.o calcd4.o cmp3.o comma.o vcliser.o \
		tida.o errq.o vattr.o vform.o vcgi2.o
	gcc  -ovpg4s \
		vpg4q.o getadq.o solsq.o calcu.o\
		$(SOLID) \
		inpq.o chefun3.o misce.o pan.o impfs.o \
		vstrings.o calcd4.o cmp3.o comma.o vcgi2.o vcliser.o \
		tida.o errq.o vattr.o vform.o \
		-lncurses  -lm  -lform -lpthread 
#	chown root vpg4s
#	chmod u+s vpg4s
	mv vpg4s bin

vcliser.o:	vcliser.c
		gcc -c vcliser.c

calcu.o:	calcu.c
		gcc -c calcu.c

vpg4q.o:	vpg4s.c vsqrun.c archisq.h
		gcc -c -DSS_UNIX -DSQLGEN -ovpg4q.o vpg4s.c

solsq.o:	solsq.c archisq.h
		gcc -c -DSS_UNIX solsq.c

getadq.o:	getad.c archisq.h
		gcc -c -DSS_UNIX -DSQLGEN -ogetadq.o getad.c

getad.o:	getad.c
		gcc -c -DVSAMD getad.c

vpg4v.o:	vpg4s.c vsamrun.c
		gcc -c -DVSAMD -ovpg4v.o vpg4s.c
		
vpsort:		vpsort.c
		gcc -obin/vpsort vpsort.c

vp:		vp.c
		gcc -obin/vp vp.c

vps:		vp3.c
		gcc -obin/vps vp3.c

vpg4.o:		vpg4.c
		gcc -c vpg4.c

vattr.o:	vattr.c
		gcc -c vattr.c

err.o:		err.c
		gcc -c err.c
		
errq.o:		err.c archisq.h
		gcc -c -DSQLGEN -oerrq.o err.c

vblo.o:		vblo.c
		gcc -c vblo.c

tida.o:		tida.c
		gcc -c tida.c

comma.o:	comma.c
		gcc -c comma.c

cmp2.o:		cmp2.c
		gcc -c cmp2.c

cmp3.o:  	cmp3.c
		gcc -c cmp3.c

calcd4.o:	calcd4.c
		gcc -c calcd4.c

vbsearch.o:	vbsearch.c
		gcc -c vbsearch.c

vstrings.o:	vstrings.c
		gcc -c vstrings.c

recup.o:	recup.c
		gcc -c recup.c

impf.o:		impf.c
		gcc -c impf.c

impfs.o:	impfs.c
		gcc -c impfs.c

pan.o:		pan.c
		gcc -c pan.c

misce.o:	misce.c
		gcc -c -DDEBUG misce.c

chefun3.o:	chefun3.c
		gcc -c chefun3.c

inpq.o:		inpq.c
		gcc -c inpq.c

vplib.o:	vplib.c
		gcc -c vplib.c

vsamfun.o:	vsamfun.c
		gcc -c vsamfun.c

cblfi:	cblfi.c
	gcc -obin/cblfi cblfi.c

vfck:	vfck.c
	gcc -obin/vfck vfck.c

sqdef:	sqdef.c archisq.h
	gcc sqdef.c /solid/lib/scll2x23.a -lm -lpthread -obin/sqdef
sqdefc:	sqdefc.c archisq.h
	gcc sqdefc.c /solid/lib/scll2x23.a -lm -lpthread -obin/sqdefc

sqdef1: sqdef1.c archisq.h
	gcc sqdef1.c /solid/lib/scllux22.a -lm -obin/sqdef1

genctr:	genctr.c archisq.h
	gcc genctr.c /solid/lib/scllux22.a -lm -obin/genctr

sqcheck: sqcheck.c archisq.h
	gcc sqcheck.c /solid/lib/scllux22.a -lm -obin/sqcheck

parcar:	parcar.c chkfecha.c vsamstr.h
	gcc parcar.c chkfecha.c -obin/parcar

parcar9:	parcar9.c vsamstr.h
	gcc parcar9.c -o/usr/bin/parcar9

parcar1: parcar1.c chkfecha.c vsamstr.h
	gcc parcar1.c chkfecha.c -obin/parcar1

parcar19:	parcar19.c vsamstr.h
	gcc parcar19.c -o/usr/bin/parcar19

parcar1a:	parcar1a.c vsamstr.h
	gcc parcar1a.c -obin/parcar1a

forpan:		forpan.c err2.c
	gcc -obin/forpan forpan.c

forpanp:	forpanp.c err2.c
	gcc -obin/forpanp forpanp.c

vform.o:	vform.c
	gcc -c vform.c

qcm:	qcm.c
	gcc qcm.c -o bin/qcm

