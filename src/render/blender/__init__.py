bl_info = {
	"name": "Mesh Format",
	"author": "Stephen Hurley",
	"blender": (2, 63, 0),
	"location": "File > Export -> Mesh Format (.msh)",
	"description": "Export Mesh, ",
	"category": "Import-Export"
}

import bpy
from bpy_extras.io_utils import ExportHelper
import copy
import struct

class Vertex:
	def __init__(self):
		self.position = [0, 0, 0]
		self.normal = [0, 0, 0]
		self.uvs = []

class Mesh:
	def __init__(self):
		self.num_indices_per_primitive = 3
		self.num_vertices_per_frame = 0
		self.format_types = [0, 2]
		self.vertices = []
		self.indices = []

def write_int(file, v):
	file.write(struct.pack('i', v))

def write_float(file, v):
	file.write(struct.pack('f', v))

def write_string(file, v):
	file.write((v + '\0').encode('utf_8'))

class ExportModel(bpy.types.Operator, ExportHelper):
	bl_idname = "default.msh"
	bl_label = "Export Mesh"
	filename_ext = ".msh"

	def execute(self, context):
		mesh = Mesh()
		bmesh = None
		for bobject in bpy.data.objects:
			if bobject.type == 'MESH':
				bmesh = bobject.data
				break
		if bmesh is None:
			raise "No mesh found!"
		file = open(self.filepath, 'wb')
		write_int(file, 3) # num indices per primitive
		write_int(file, 0) # num vertices per frame
		write_int(file, len([0, 2])) # format types: position_xyz, normal
		write_int(file, 0) # format types: position_xyz, normal
		write_int(file, 2) # format types: position_xyz, normal
		write_int(file, len(bmesh.vertices) * 6)
		for bvertex in bmesh.vertices:
			write_float(file, bvertex.co[0])
			write_float(file, bvertex.co[1])
			write_float(file, bvertex.co[2])
			write_float(file, bvertex.normal[0])
			write_float(file, bvertex.normal[1])
			write_float(file, bvertex.normal[2])
		write_int(file, len(bmesh.polygons) * 3)
		for bpolygon in bmesh.polygons:
			if len(bpolygon.vertices) != 3:
				raise "Not all faces are triangles! You must TRIANGULATE! (edit mode -> ctrl-t)"
			write_int(file, bpolygon.vertices[0])
			write_int(file, bpolygon.vertices[1])
			write_int(file, bpolygon.vertices[2])
		file.close()
		# if len(bmesh.materials) > 0:
			# bmaterial = bmesh.materials[0]
			# mesh.material.diffuse_color[0] = bmaterial.diffuse_color[0]
			# mesh.material.diffuse_color[1] = bmaterial.diffuse_color[1]
			# mesh.material.diffuse_color[2] = bmaterial.diffuse_color[2]
			# mesh.material.shininess = bmaterial.specular_hardness
			# mesh.material.shininess_strength = bmaterial.specular_intensity
			# for texture_slot in bmaterial.texture_slots:
				# if texture_slot is not None and texture_slot.use == True and texture_slot.texture.type == 'IMAGE' and texture_slot.texture.image is not None:
					# texture = Texture()
					# texture.filename = texture_slot.texture.image.filepath
					# if texture_slot.use_map_color_diffuse == True:
						# texture.type = 'diffuse'
					# if texture_slot.texture_coords == 'UV':
						# uv_layer_i = 0
						# for uv_layer in bmesh.uv_layers:
							# if uv_layer.name == texture_slot.uv_layer:
								# texture.uv_index = uv_layer_i
							# uv_layer_i += 1
					# mesh.material.textures.append(texture)
		# for bvertex in bmesh.vertices:
			# mesh.vertices.append(bvertex.co[0]);
			# mesh.vertices.append(bvertex.co[1]);
			# mesh.vertices.append(bvertex.co[2]);
			# mesh.vertices.append(bvertex.normal[0]);
			# mesh.vertices.append(bvertex.normal[1]);
			# mesh.vertices.append(bvertex.normal[2]);
		# for bpolygon in bmesh.polygons:
			# if len(bpolygon.vertices) != 3:
				# raise "Not all faces are triangles! You must TRIANGULATE! (edit mode -> ctrl-t)"
			# mesh.indices.append(bpolygon.vertices[0])
			# mesh.indices.append(bpolygon.vertices[1])
			# mesh.indices.append(bpolygon.vertices[2])
		# mesh.num_uvs = len(bmesh.uv_layers)
		# for uv_layer_i in range(0, len(bmesh.uv_layers)):
			# uv_layer = bmesh.uv_layers[uv_layer_i]
			# for uv_loop_i in range(0, len(uv_layer.data)):
				# uv = uv_layer.data[uv_loop_i].uv
				# vertex = mesh.vertices[mesh.indices[uv_loop_i]]
				# if uv_layer_i < len(vertex.uvs) and (uv[0] != vertex.uvs[uv_layer_i][0] or uv[1] != vertex.uvs[uv_layer_i][1]): # if true, split vertex
					# new_vertex = copy.deepcopy(vertex)
					# new_vertex.uvs[uv_layer_i][0] = uv[0]
					# new_vertex.uvs[uv_layer_i][1] = uv[1]
					# mesh.vertices.append(new_vertex)
					# mesh.indices[uv_loop_i] = len(mesh.vertices) - 1
				# elif uv_layer_i == len(vertex.uvs):
					# vertex.uvs.append([])
					# vertex.uvs[uv_layer_i].append(uv[0])
					# vertex.uvs[uv_layer_i].append(uv[1])
		# file = open(self.filepath, 'wb')
		# write_string(file, mesh.name)
		# write_float(file, mesh.material.diffuse_color[0])
		# write_float(file, mesh.material.diffuse_color[1])
		# write_float(file, mesh.material.diffuse_color[2])
		# write_float(file, mesh.material.diffuse_color[3])
		# write_int(file, mesh.material.shininess)
		# write_float(file, mesh.material.shininess_strength)
		# write_int(file, len(mesh.material.textures))
		# for texture in mesh.material.textures:
			# write_string(file, texture.filename)
			# write_string(file, texture.type)
			# write_int(file, texture.uv_index)
		# write_bool(file, True) # has normals
		# write_bool(file, False) # has no tangent
		# write_bool(file, False) # has no color
		# write_int(file, mesh.num_uvs)
		# write_int(file, len(mesh.vertices))
		# for vertex in mesh.vertices:
			# write_float(file, vertex.position[0])
			# write_float(file, vertex.position[1])
			# write_float(file, vertex.position[2])
			# write_float(file, vertex.normal[0])
			# write_float(file, vertex.normal[1])
			# write_float(file, vertex.normal[2])
			# for uv in vertex.uvs:
				# write_float(file, uv[0])
				# write_float(file, uv[1])
		# write_int(file, 3) # num indices per primitive = triangles
		# write_int(file, len(mesh.indices))
		# for index in mesh.indices:
			# write_int(file, index)
		return {'FINISHED'}

def menu_func_export(self, context):
	self.layout.operator(ExportModel.bl_idname, text="Mesh Format (.msh)")

def register():
	bpy.utils.register_module(__name__)
	bpy.types.INFO_MT_file_export.append(menu_func_export)

def unregister():
	bpy.utils.unregister_module(__name__)
	bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
	register()

