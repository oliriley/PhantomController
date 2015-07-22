# Makefile - QuickHaptics Examples

#export DEBUG = TRUE

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

.PHONY: all
all: \
	ComplexScene	\
	EarthSpin	\
	pickApples	\
	ShapeDepthFeedback	\
	SimpleSphere	\
	SkullCoulombForce		\
	SpongyCow		\
	TeethCavityPick			

.PHONY: ComplexScene
ComplexScene:
	$(MAKE) -C ComplexScene/ComplexSceneGLUT -f ComplexSceneGLUT.mak

.PHONY: EarthSpin
EarthSpin:
	$(MAKE) -C EarthSpin/EarthSpinGLUT -f EarthSpinGLUT.mak

.PHONY: pickApples
pickApples:
	$(MAKE) -C pickApples/pickApplesGLUT  -f pickApplesGLUT.mak

.PHONY: ShapeDepthFeedback
ShapeDepthFeedback:
	$(MAKE) -C ShapeDepthFeedback/ShapeDepthFeedbackGLUT -f ShapeDepthFeedbackGLUT.mak

.PHONY: SimpleSphere
SimpleSphere:
	$(MAKE) -C SimpleSphere/SimpleSphereGLUT -f SimpleSphereGLUT.mak

.PHONY: SkullCoulombForce
SkullCoulombForce:
	$(MAKE) -C SkullCoulombForce/SkullCoulombForceGLUT -f SkullCoulombForceGLUT.mak

.PHONY: SpongyCow
SpongyCow:
	$(MAKE) -C SpongyCow/SpongyCowGLUT -f SpongyCowGLUT.mak

.PHONY: TeethCavityPick
TeethCavityPick:
	$(MAKE) -C TeethCavityPick/TeethCavityPickGLUT -f TeethCavityPickGLUT.mak

.PHONY: clean
clean:
	$(MAKE) -C ComplexScene/ComplexSceneGLUT -f ComplexSceneGLUT.mak clean
	$(MAKE) -C EarthSpin/EarthSpinGLUT -f EarthSpinGLUT.mak clean
	$(MAKE) -C pickApples/pickApplesGLUT  -f pickApplesGLUT.mak clean
	$(MAKE) -C ShapeDepthFeedback/ShapeDepthFeedbackGLUT -f ShapeDepthFeedbackGLUT.mak clean
	$(MAKE) -C SimpleSphere/SimpleSphereGLUT -f SimpleSphereGLUT.mak clean
	$(MAKE) -C SkullCoulombForce/SkullCoulombForceGLUT -f SkullCoulombForceGLUT.mak clean
	$(MAKE) -C SpongyCow/SpongyCowGLUT -f SpongyCowGLUT.mak clean
	$(MAKE) -C TeethCavityPick/TeethCavityPickGLUT -f TeethCavityPickGLUT.mak clean


.PHONY: install
install:
	$(MAKE) -C ComplexScene/ComplexSceneGLUT -f ComplexSceneGLUT.mak install 
	$(MAKE) -C EarthSpin/EarthSpinGLUT -f EarthSpinGLUT.mak install
	$(MAKE) -C pickApples/pickApplesGLUT  -f pickApplesGLUT.mak install
	$(MAKE) -C ShapeDepthFeedback/ShapeDepthFeedbackGLUT -f ShapeDepthFeedbackGLUT.mak install
	$(MAKE) -C SimpleSphere/SimpleSphereGLUT -f SimpleSphereGLUT.mak install
	$(MAKE) -C SkullCoulombForce/SkullCoulombForceGLUT -f SkullCoulombForceGLUT.mak install
	$(MAKE) -C SpongyCow/SpongyCowGLUT -f SpongyCowGLUT.mak install
	$(MAKE) -C TeethCavityPick/TeethCavityPickGLUT -f TeethCavityPickGLUT.mak install
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/../QuickHaptics/examples/

