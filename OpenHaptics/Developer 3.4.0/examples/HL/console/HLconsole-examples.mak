# Makefile - ConsoleExamples

#export DEBUG = TRUE

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

.PHONY: all
all: \
	CannedForceEffect \
	CustomForceEffect \
	CustomShape \
	Deployment \
	EffectAttributes

.PHONY: CannedForceEffect
CannedForceEffect:
	$(MAKE) -C CannedForceEffect -f CannedForceEffect.mak

.PHONY: CustomForceEffect
CustomForceEffect:
	$(MAKE) -C CustomForceEffect -f CustomForceEffect.mak

.PHONY: CustomShape
CustomShape:
	$(MAKE) -C CustomShape -f CustomShape.mak

.PHONY: Deployment
Deployment:
	$(MAKE) -C Deployment -f Deployment.mak

.PHONY: EffectAttributes
EffectAttributes:
	$(MAKE) -C EffectAttributes -f EffectAttributes.mak

.PHONY: clean
clean:
	$(MAKE) -C CannedForceEffect -f CannedForceEffect.mak clean
	$(MAKE) -C CustomForceEffect -f CustomForceEffect.mak clean
	$(MAKE) -C CustomShape -f CustomShape.mak clean
	$(MAKE) -C Deployment -f Deployment.mak clean
	$(MAKE) -C EffectAttributes -f EffectAttributes.mak clean

.PHONY: install
install:
	$(MAKE) -C CannedForceEffect -f CannedForceEffect.mak install
	$(MAKE) -C CustomForceEffect -f CustomForceEffect.mak install
	$(MAKE) -C CustomShape -f CustomShape.mak install
	$(MAKE) -C Deployment -f Deployment.mak install
	$(MAKE) -C EffectAttributes -f EffectAttributes.mak install
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/HL/console/
#	install -m 755 -o 0 -g 0 run_hl_console_examples.pl $(examples_dir)/HL/console/


