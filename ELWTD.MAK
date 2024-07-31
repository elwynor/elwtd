#################################################################################
#####                 This makefile generates ELWTD.DLL                     #####
#################################################################################
!include $(WGDEV)\src\gcrules.mak

VIRDIR = .\GCVIRDAT

!if $(OS) == DOS
!    if $(MODEL) == LARGE
!         error ELWTD not supported in Large Model under DOS!
!    endif
     DLLLIB = $(DLLLIB) $(COMPILER)\lib\mathh.lib
     PRODLL = $(PRODLL) $(DOSEXT)\bc5\lib\fpdmy
!else
     PARSEOPTS = $(PARSEOPTS)
!endif

!if $(OS) == WNT
DLLLIB = $(DLLLIB) wgserver.lib galgsbl.lib
PRODLL =
DEFILE =
PARSEOPTS = $(PARSEOPTS) -I. -DIS30
!endif

!if $(OS) == WG2
DLLLIB = $(DLLLIB) wgserver.lib galgsbl.lib $(BORLAND)\lib\mathh.lib ${DOSEXT}\bc4\lib\phapi.lib
PRODLL = $(DOSEXT)\bc4\lib\fpdmy
DEFILE = $(WGLIBDIR)\mathdef.def
PARSEOPTS = $(PARSEOPTS) -I.
!endif

VIRFILES =          \
     GCVIRDAT\elwtdsco.vir \
     GCVIRDAT\elwtdusr.vir

OBJFILES = \
     ${TEMPDIR}\elwtd.obj 

start:              \
     $(GCBUILD)     \
     elwtd.dll

elwtd.dll:          \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|
