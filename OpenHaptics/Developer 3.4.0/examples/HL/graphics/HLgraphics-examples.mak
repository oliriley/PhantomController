# Makefile - GraphicsExamples

#export DEBUG = TRUE

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

.PHONY: all
all: \
	Constraints \
	Events \
	HelloHaptics \
	HelloSphere \
	HelloSphereDual \
	HL_DOP_Demo \
	PointManipulation \
	ShapeManipulation \
	SimpleDeformableSurface \
	SimplePinchDemo \
	SimpleRigidBodyDynamics
#	HapticMaterials \

.PHONY: Constraints
Constraints:
	$(MAKE) -C Constraints -f Constraints.mak

.PHONY: Events
Events:
	$(MAKE) -C Events -f Events.mak

#.PHONY: HapticMaterials
#HapticMaterials:
#	$(MAKE) -C HapticMaterials -f HapticMaterials.mak

.PHONY: HelloHaptics
HelloHaptics:
	$(MAKE) -C HelloHaptics -f HelloHaptics.mak

.PHONY: HelloSphere
HelloSphere:
	$(MAKE) -C HelloSphere -f HelloSphere.mak

.PHONY: HelloSphereDual
HelloSphereDual:
	$(MAKE) -C HelloSphereDual -f HelloSphereDual.mak

.PHONY: HL_DOP_Demo
HL_DOP_Demo:
	$(MAKE) -C HL_DOP_Demo -f HL_DOP_Demo.mak

.PHONY: PointManipulation
PointManipulation:
	$(MAKE) -C PointManipulation -f PointManipulation.mak

.PHONY: ShapeManipulation
ShapeManipulation:
	$(MAKE) -C ShapeManipulation -f ShapeManipulation.mak

.PHONY: SimpleDeformableSurface
SimpleDeformableSurface:
	$(MAKE) -C SimpleDeformableSurface -f SimpleDeformableSurface.mak

.PHONY: SimplePinchDemo
SimplePinchDemo:
	$(MAKE) -C SimplePinchDemo -f SimplePinchDemo.mak

.PHONY: SimpleRigidBodyDynamics
SimpleRigidBodyDynamics:
	$(MAKE) -C SimpleRigidBodyDynamics -f SimpleRigidBodyDynamics.mak

.PHONY: HL_DOP_Demo
HL_DOP_Demo:
	$(MAKE) -C HL_DOP_Demo -f HL_DOP_Demo.mak

.PHONY: SimplePinchDemo
SimplePinchDemo:
	$(MAKE) -C SimplePinchDemo -f SimplePinchDemo.mak

.PHONY: clean
clean:
	$(MAKE) -C Constraints -f Constraints.mak clean
	$(MAKE) -C Events -f Events.mak clean
#	$(MAKE) -C HapticMaterials -f HapticMaterials.mak clean
	$(MAKE) -C HelloHaptics -f HelloHaptics.mak clean
	$(MAKE) -C HelloSphere -f HelloSphere.mak clean
	$(MAKE) -C HelloSphereDual -f HelloSphereDual.mak clean
	$(MAKE) -C HL_DOP_Demo -f HL_DOP_Demo.mak clean
	$(MAKE) -C PointManipulation -f PointManipulation.mak clean
	$(MAKE) -C ShapeManipulation -f ShapeManipulation.mak clean
	$(MAKE) -C SimpleDeformableSurface -f SimpleDeformableSurface.mak clean
	$(MAKE) -C SimplePinchDemo -f SimplePinchDemo.mak clean
	$(MAKE) -C SimpleRigidBodyDynamics -f SimpleRigidBodyDynamics.mak clean
	$(MAKE) -C HL_DOP_Demo -f HL_DOP_Demo.mak clean
	$(MAKE) -C SimplePinchDemo -f SimplePinchDemo.mak clean

.PHONY: install
install:
	$(MAKE) -C Constraints -f Constraints.mak install
	$(MAKE) -C Events -f Events.mak install
	$(MAKE) -C HelloHaptics -f HelloHaptics.mak install
	$(MAKE) -C HelloSphere -f HelloSphere.mak install
	$(MAKE) -C HelloSphereDual -f HelloSphereDual.mak install
	$(MAKE) -C HL_DOP_Demo -f HL_DOP_Demo.mak install
	$(MAKE) -C PointManipulation -f PointManipulation.mak install
	$(MAKE) -C ShapeManipulation -f ShapeManipulation.mak install
	$(MAKE) -C SimpleDeformableSurface -f SimpleDeformableSurface.mak install
	$(MAKE) -C SimplePinchDemo -f SimplePinchDemo.mak install
	$(MAKE) -C SimpleRigidBodyDynamics -f SimpleRigidBodyDynamics.mak install
	$(MAKE) -C HL_DOP_Demo -f HL_DOP_Demo.mak install
	$(MAKE) -C SimplePinchDemo -f SimplePinchDemo.mak install
#	$(MAKE) -C HapticMaterials -f HapticMaterials.mak install
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/HL/graphics/
#	install -m 755 -o 0 -g 0 run_hl_graphics_examples.pl $(examples_dir)/HL/graphics


