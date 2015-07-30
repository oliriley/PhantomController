# Makefile - GraphicsExamples

#export DEBUG = TRUE

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

.PHONY: all
all: \
	CoulombField \
	CoulombForceDual \
	ParticleWaltz \
	PointManipulation \
	PointSnapping \
	SimpleHapticScene \
	SlidingContact

.PHONY: CoulombField
CoulombField:
	$(MAKE) -C CoulombField -f CoulombField.mak

.PHONY: CoulombForceDual
CoulombForceDual:
	$(MAKE) -C CoulombForceDual -f CoulombForceDual.mak

.PHONY: ParticleWaltz
ParticleWaltz:
	$(MAKE) -C ParticleWaltz -f ParticleWaltz.mak

.PHONY: PointManipulation
PointManipulation:
	$(MAKE) -C PointManipulation -f PointManipulation.mak

.PHONY: PointSnapping
PointSnapping:
	$(MAKE) -C PointSnapping -f PointSnapping.mak

.PHONY: SimpleHapticScene
SimpleHapticScene:
	$(MAKE) -C SimpleHapticScene -f SimpleHapticScene.mak

.PHONY: SlidingContact
SlidingContact:
	$(MAKE) -C SlidingContact -f SlidingContact.mak

.PHONY: clean
clean:
	$(MAKE) -C CoulombField -f CoulombField.mak clean
	$(MAKE) -C CoulombForceDual -f CoulombForceDual.mak clean
	$(MAKE) -C ParticleWaltz -f ParticleWaltz.mak clean
	$(MAKE) -C PointManipulation -f PointManipulation.mak clean
	$(MAKE) -C PointSnapping -f PointSnapping.mak clean
	$(MAKE) -C SimpleHapticScene -f SimpleHapticScene.mak clean
	$(MAKE) -C SlidingContact -f SlidingContact.mak clean

.PHONY: install
install:
	$(MAKE) -C CoulombField -f CoulombField.mak install
	$(MAKE) -C CoulombForceDual -f CoulombForceDual.mak install
	$(MAKE) -C ParticleWaltz -f ParticleWaltz.mak install
	$(MAKE) -C PointManipulation -f PointManipulation.mak install
	$(MAKE) -C PointSnapping -f PointSnapping.mak install
	$(MAKE) -C SimpleHapticScene -f SimpleHapticScene.mak install
	$(MAKE) -C SlidingContact -f SlidingContact.mak install
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/HD/graphics/
#	install -m 755 -o 0 -g 0 run_hd_graphics_examples.pl $(examples_dir)/HD/graphics/


