# -*- coding: iso-8859-1 -*-
# Copyright (C) 2004-2009 The PaGMO development team,
# Advanced Concepts Team (ACT), European Space Agency (ESA)
# http://apps.sourceforge.net/mediawiki/pagmo
# http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers
# http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits
# act@esa.int
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the
# Free Software Foundation, Inc.,
# 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
  
## @package environment
#  This module contains the ALifeEnvironment class. It loads the robot and
#  asteroid models and puts them into an ODE world. The ODE world handles all 
#  physics simulation.
#  Code is based on the ode.environment module from PyBrain
from pybrain.rl.environments.ode import sensors, actuators
from pybrain.rl.environments.ode.tools.configgrab import ConfigGrabber
import ode
import xode.parser
import xml.dom.minidom as md

## ALifeEnvironment class
#
#  Handles all Physics simulation. 
#  It loads the robot and asteroid models and puts them into an ODE world. 
#  Code is based on the ode.environment module from PyBrain
#
#  @author John Glover
class ALifeEnvironment(object):
    def __init__(self):
        ## @var root XODE root node, defined in load_xode
        self.root = None
        ## @var world XODE world node, defined in load_xode
        self.world = None 
        ## @var space:  XODE space node, defined in load_xode
        self.space = None  
        ## @var body_geom A list with (body, geom) tuples
        self.body_geom = []
        ## @var asteroid The asteroid geometry
        self.asteroid = None
        ## @var textures the textures dictionary
        self.textures = {}
        ## @var sensors sensor list
        self.sensors = []
        ## @var excludesensors list of sensors to exclude
        self.excludesensors = []
        ## @var actuators actuators list
        self.actuators = []
        ## @var contactgroup A joint group for the contact joints that 
        # are generated whenever two bodies collide
        self.contactgroup = ode.JointGroup()
        self.FricMu = 8.0
        self.steps_per_action = 1
        self.step_count = 0
    
    ## Loads the robot XODE file (xml format) and parses it.
    #  @param file_name The file path to the .xode file for the robot.
    #  @param reload Whether or not to reload sensor data .
    def load_xode(self, file_name, reload=False):
        f = file(file_name)
        p = xode.parser.Parser()
        self.root = p.parseFile(f)
        f.close()
        try:
            # filter all xode "world" objects from root, take only the first one
            world = filter(lambda x: isinstance(x, xode.parser.World), self.root.getChildren())[0]
        except IndexError:
            # malicious format, no world tag found
            print "no <world> tag found in " + file_name + ". quitting."
            sys.exit()
        self.world = world.getODEObject()
        self.world.setGravity((0, -9.81, 0))
        try:
            # filter all xode "space" objects from world, take only the first one
            space = filter(lambda x: isinstance(x, xode.parser.Space), world.getChildren())[0]
        except IndexError:
            # malicious format, no space tag found
            print "no <space> tag found in " + file_name + ". quitting."
            sys.exit()
        self.space = space.getODEObject()
                
        # load bodies and geoms for painting
        self.body_geom = [] 
        self._parseBodies(self.root)

        # now parse the additional parameters at the end of the xode file
        self._loadConfig(file_name, reload)
        
    ## Loads the asteroid X3D file (XML) and parses it. The resulting
    #  geometry is a xode trimesh object, stored in the variable self.asteroid.
    #  The file must contain an object called 'Asteroid'.
    #  If more than 1 object called 'Asteroid' exists, only the first one
    #  is processed, the rest are ignored.
    #  The file must consist of faces stored as triangles. If the file 
    #  cannot be read or it does not contain any triangles an exception
    #  is raised.
    #  @param file_name The file path to the .x3d file for the asteroid.
    def load_asteroid(self, file_name):
        dom = md.parse(file_name)
        root = dom.createElement('xode')
        root.attributes['version'] = '1.0r23'
        root.attributes['name'] = 'alife'
        root.attributes['xmlns:xsi'] = 'http://www.w3.org/2001/XMLSchema-instance'
        root.attributes['xsi:noNamespaceSchemaLocation'] = 'http://tanksoftware.com/xode/1.0r23/xode.xsd'
        world = dom.createElement('world')
        root.appendChild(world)
        space = dom.createElement('space')
        world.appendChild(space)
        
        geom = dom.createElement('geom')
        geom.attributes['name'] = 'Asteroid'
        trimesh = dom.createElement('trimesh')
        geom.appendChild(trimesh)
        trimesh_triangles = dom.createElement('triangles')
        trimesh.appendChild(trimesh_triangles)
        trimesh_vertices = dom.createElement('vertices')
        trimesh.appendChild(trimesh_vertices)
        space.appendChild(geom)
        
        for node in dom.getElementsByTagName('Transform'):
            if 'DEF' in node.attributes.keys():
                # take the first transform node defined as 'Asteroid'
                if node.attributes['DEF'].value == 'Asteroid':
                    # get scale information from the model
                    if 'scale' in node.attributes.keys():
                        scale_string = node.attributes['scale'].value
                        scale = scale_string.split()
                        scale = [float(s) for s in scale]
                    else:
                        scale = (1, 1, 1)
                        
                    # todo: get translation information from the model
                    # todo: get rotation information from the model
                    
                    if node.getElementsByTagName('IndexedFaceSet'):
                        ifs = node.getElementsByTagName('IndexedFaceSet')[0]
                        # form triangles from the coordIndex
                        coord_index = ifs.attributes['coordIndex'].value
                        for face in coord_index.split(','):
                            # make sure that the given faces are triangles
                            # there should 4 indicies, the last one equal to -1
                            indicies = face.split()
                            if len(indicies) == 4 and int(indicies[3]) == -1:
                                # x3d indices count from zero but xode indices
                                # count form one, so add one to each index value
                                t = dom.createElement('t')
                                t.attributes['ia'] = str(int(indicies[0])+1)
                                t.attributes['ib'] = str(int(indicies[1])+1)
                                t.attributes['ic'] = str(int(indicies[2])+1)
                                trimesh_triangles.appendChild(t)
                        # form vertices from the Coordinate point attribute
                        coordinate = ifs.getElementsByTagName('Coordinate')[0]
                        coord_points = coordinate.attributes['point'].value
                        for points in coord_points.split(','):
                            # each vertex should have 3 points
                            points = points.split()
                            if len(points) == 3:
                                v = dom.createElement('v')
                                v.attributes['x'] = str(float(points[0]) * scale[0])
                                v.attributes['y'] = str(float(points[1]) * scale[1])
                                v.attributes['z'] = str(float(points[2]) * scale[2])
                                trimesh_vertices.appendChild(v)
                        break
    
        # parse, adding to environment
        parser = xode.parser.Parser()
        self._parseBodies(parser.parseString(root.toxml()))
        
        # check that asteroid geometry was created successfully
        # todo: check triangle count
        # todo: give more detail in exception
        if not self.asteroid:
            raise Exception("NoAsteroid")
        else:
            print "Asteroid created, %d triangles" % (self.asteroid.getTriangleCount())
    
    ## Load the XODE config.
    #  @param filename The file path to the config file (XODE file).
    #  @param reload Whether or not to reload sensor data.  
    def _loadConfig(self, filename, reload=False):
        # parameters are given in (our own brand of) config-file syntax
        self.config = ConfigGrabber(filename, sectionId="<!--odeenvironment parameters", delim=("<", ">"))

        # <passpairs>
        self.passpairs = []
        for passpairstring in self.config.getValue("passpairs")[:]:
            self.passpairs.append(eval(passpairstring))

        # <affixToEnvironment>
        for jointName in self.config.getValue("affixToEnvironment")[:]:
            try:
                # find first object with that name
                obj = self.root.namedChild(jointName).getODEObject()
            except IndexError:
                print "ERROR: Could not affix object '" + jointName + "' to environment!"
                sys.exit(1)
            if isinstance(obj, ode.Joint):
                # if it is a joint, use this joint to fix to environment
                obj.attach(obj.getBody(0), ode.environment)
            elif isinstance(obj, ode.Body):
                # if it is a body, create new joint and fix body to environment
                j = ode.FixedJoint(self.world)
                j.attach(obj, ode.environment)
                j.setFixed()

        # <colors>
        for coldefstring in self.config.getValue("colors")[:]:
            # ('name', (0.3,0.4,0.5))
            objname, coldef = eval(coldefstring)
            for (body, _) in self.body_geom:
                if hasattr(body, 'name'):
                    if objname == body.name:
                        body.color = coldef
                        break
                
        if not reload:
            # add the JointSensor as default
            self.sensors = [] 
            ## self.addSensor(self._jointSensor)
            
            # <sensors>
            # expects a list of strings, each of which is the executable command to create a sensor object
            # example: DistToPointSensor('legSensor', (0.0, 0.0, 5.0))
            sens = self.config.getValue("sensors")[:]
            for s in sens:
                try:
                    self.addSensor(eval('sensors.' + s))
                except AttributeError:
                    print dir(sensors)
                    warnings.warn("Sensor name with name " + s + " not found. skipped.")
        else:
            for s in self.sensors:
                s._connect(self)
            for a in self.actuators:
                a._connect(self)

    ## Parse the given xode node and all children (recursively), creating ODE body and geometry objects.
    #  @param node The XODE node.
    def _parseBodies(self, node):
        # body (with nested geom)
        if isinstance(node, xode.body.Body):
            body = node.getODEObject()
            body.name = node.getName()
            try:
                # filter all xode geom objects and take the first one
                xgeom = filter(lambda x: isinstance(x, xode.geom.Geom), node.getChildren())[0]
            except IndexError:
                return() # no geom object found, skip this node
            # get the real ode object
            geom = xgeom.getODEObject()
            # if geom doesn't have own name, use the name of its body
            geom.name = node.getName()
            self.body_geom.append((body, geom))
        
        # geom on its own without body
        elif isinstance(node, xode.geom.Geom):
            try:
                node.getFirstAncestor(ode.Body)
            except xode.node.AncestorNotFoundError:
                body = None
                geom = node.getODEObject()
                geom.name = node.getName()
                self.body_geom.append((body, geom))
                if geom.name == "Asteroid":
                    self.asteroid = geom
        
        # special cases for joints: universal, fixed, amotor
        elif isinstance(node, xode.joint.Joint):
            joint = node.getODEObject()
            
            if type(joint) == ode.UniversalJoint:
                # insert an additional AMotor joint to read the angles from and to add torques
                # amotor = ode.AMotor(self.world)
                # amotor.attach(joint.getBody(0), joint.getBody(1))
                # amotor.setNumAxes(3)
                # amotor.setAxis(0, 0, joint.getAxis2())
                # amotor.setAxis(2, 0, joint.getAxis1())
                # amotor.setMode(ode.AMotorEuler)
                # xode_amotor = xode.joint.Joint(node.getName() + '[amotor]', node.getParent())
                # xode_amotor.setODEObject(amotor)
                # node.getParent().addChild(xode_amotor, None)
                pass
            if type(joint) == ode.AMotor:
                # do the euler angle calculations automatically (ref. ode manual)
                joint.setMode(ode.AMotorEuler)
                
            if type(joint) == ode.FixedJoint:
                # prevent fixed joints from bouncing to center of first body
                joint.setFixed()

        # recursive call for all child nodes
        for c in node.getChildren():
            self._parseBodies(c)
            
    ## Callback function for the collide() method.
    #  This function checks if the given geoms do collide and 
    #  creates contact joints if they do.
    #  @param args args
    #  @param geom1 geom1
    #  @param geom2 geom2  
    def _near_callback(self, args, geom1, geom2):
        # only check parse list, if objects have name
        if geom1.name != None and geom2.name != None:
            # Preliminary checking, only collide with certain objects
            for p in self.passpairs:
                g1 = False
                g2 = False
                for x in p:
                    g1 = g1 or (geom1.name.find(x) != -1)
                    g2 = g2 or (geom2.name.find(x) != -1)
                if g1 and g2:
                    return()

        # Check if the objects do collide
        contacts = ode.collide(geom1, geom2)
        
        # Create contact joints
        world, contactgroup = args
        for c in contacts:
            p = c.getContactGeomParams()
            # parameters from Niko Wolf
            c.setBounce(0.2)
            c.setBounceVel(0.05) #Set the minimum incoming velocity necessary for bounce
            c.setSoftERP(0.6) #Set the contact normal "softness" parameter
            c.setSoftCFM(0.00005) #Set the contact normal "softness" parameter
            c.setSlip1(0.02) #Set the coefficient of force-dependent-slip (FDS) for friction direction 1
            c.setSlip2(0.02) #Set the coefficient of force-dependent-slip (FDS) for friction direction 2
            c.setMu(self.FricMu) #Set the Coulomb friction coefficient
            j = ode.ContactJoint(world, contactgroup, c)
            j.name = None
            j.attach(geom1.getBody(), geom2.getBody())
            
    ## Get the list of body and geometry objects.
    #  @return list of (body, geometry) tuples.
    def get_objects(self):
        return self.body_geom
            
    ## Calculate the next step in the ODE environment.
    #  @param dt The step size. 
    def step(self, dt=0.04):
        """ Here the ode physics is calculated by one step. """
        # Detect collisions and create contact joints
        self.space.collide((self.world, self.contactgroup), self._near_callback)

        # Simulation step
        self.world.step(dt)
        
        # Remove all contact joints
        self.contactgroup.empty()
            
        # update all sensors
        for s in self.sensors:
            s._update()
        
        # increase step counter
        self.step_count += 1
        return self.step_count