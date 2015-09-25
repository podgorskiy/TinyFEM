import sys
import os
import re
import xml.etree.ElementTree as ET
from xml.etree.ElementTree import Element, SubElement, Comment, tostring
from xml.etree import ElementTree
from xml.dom import minidom

def prettify(elem):
    """Return a pretty-printed XML string for the Element.
    """
    rough_string = ElementTree.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="  ")

def GetContent():
	with open("AbaqusFiles/PlateWithHole_CPS4.inp") as f:
		return f.readlines()

class AbaqusReader:

	def __init__(self):
		self.lines = GetContent()
		self.currentLine = 0
		self.nodes = []
		self.elements = []

	def BackUp(self):
		self.backup = self.currentLine
		
	def Restore(self):
		self.currentLine = self.backup

	def EatLine(self):
		if len(self.lines) > self.currentLine:
			line = self.lines[self.currentLine]
			self.currentLine+=1
			return (True, line[:-1])
		return (False, "")
		
	def AcceptLine(self, lineToAccept):
		p = re.compile(lineToAccept, re.IGNORECASE)
		str_ = ""
		while True:
			(ok, str_) = self.EatLine()
			if not ok:
				return (False, str_)
			if p.match(str_):
				print str_
				return (True, str_)
			if str_[0:2] == "**":
				continue
			break
		return (False, str_)
			
	def ReadFile(self):
		self.AcceptLine("\*Heading")
		self.AcceptLine("\*Preprint")
		self.AcceptLine("\*Part")
		self.AcceptLine("\*Node")
		str_ = ""
		nodeReader = re.compile("\s*(?P<nodeI>\d*)\s*,\s*(?P<X>[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?)\s*,\s*(?P<Y>[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?)")
		while True:
			(ok, str_) = self.AcceptLine("\*Element")
			if not ok:
				m = nodeReader.match(str_)
				if m:
					i = int(m.group('nodeI'))
					x = float(m.group('X'))
					y = float(m.group('Y'))
					self.nodes.append((i-1,x,y))
			else:
				break
		elementTypeRe = re.compile("\*Element, type=(?P<element>\S*)")
		em = elementTypeRe.match(str_)
		currentElement = em.group("element")
		elementCPS3_reader = re.compile("\s*(?P<eI>\d*)\s*,\s*(?P<n1>\d*)\s*,\s*(?P<n2>\d*)\s*,\s*(?P<n3>\d*)")
		elementCPS4_reader = re.compile("\s*(?P<eI>\d*)\s*,\s*(?P<n1>\d*)\s*,\s*(?P<n2>\d*)\s*,\s*(?P<n3>\d*),\s*(?P<n4>\d*)")
		while True:
			self.BackUp()
			(ok, str_) = self.AcceptLine("\*End Part")
			if not ok:
				self.Restore()
				while True:
					(ok, str_) = self.AcceptLine("\*Element")
					if not ok:
						if currentElement == "CPS3":
							m = elementCPS3_reader.match(str_)
							if m:
								i = int(m.group('eI'))
								n1 = int(m.group('n1'))
								n2 = int(m.group('n2'))
								n3 = int(m.group('n3'))
								self.elements.append((i-1,[n1-1, n2-1, n3-1], "CPS3"))
							else:
								break
						if currentElement == "CPS4":
							m = elementCPS4_reader.match(str_)
							if m:
								i = int(m.group('eI'))
								n1 = int(m.group('n1'))
								n2 = int(m.group('n2'))
								n3 = int(m.group('n3'))
								n4 = int(m.group('n4'))
								self.elements.append((i-1,[n1-1, n2-1, n3-1, n4-1], "CPS4"))
							else:
								break
					else:
						em = elementTypeRe.match(str_)
						currentElement = em.group("element")
						break
			else:
				break

		top = Element('FEM_INPUT')
		materials = SubElement(top, 'materials')
		material = SubElement(materials, 'material', {"id":"steel"})
		field1 = SubElement(material, 'field', {"name":"PoissonsRatio", "value": "0.3"})
		field2 = SubElement(material, 'field', {"name":"YoungsModulus", "value": "2000"})

		NodeList = SubElement(top, 'NodeList', {"count": str(len(self.nodes))})
		for node in self.nodes:
			(i, x, y) = node
			Node = SubElement(NodeList, 'node', {"i": str(i), "x": str(x), "y": str(y)})
			
		ElementList = SubElement(top, 'ElementList', {"count": str(len(self.elements))})
		for element in self.elements:
			(i, ns, type) = element
			if type == "CPS3":
				el = SubElement(ElementList, 'element', {"i": str(i), "indices": str(ns[0]) + ' ' + str(ns[1]) + ' ' + str(ns[2]), "type":"CPS3" })
			if type == "CPS4":
				el = SubElement(ElementList, 'element', {"i": str(i), "indices": str(ns[0]) + ' ' + str(ns[1]) + ' ' + str(ns[2]) + ' ' + str(ns[3]), "type":"CPS4" })

		text_file = open("PlateWithHole_CPS4.inp", "w")
		text_file.write(prettify(top))
		text_file.close()


convertor = AbaqusReader()
convertor.ReadFile()