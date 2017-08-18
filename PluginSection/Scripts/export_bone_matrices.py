#----------------------------------------------------------
# Export bone matrices as c++ source code
# Simple helper to export Oculus animation data from
# .fbx files imported into Blender
#----------------------------------------------------------
import bpy
import os

#
#    Menu in toolprops region
#
class ToolPropsPanel(bpy.types.Panel):
    bl_label = "Export bone matrices"
    bl_space_type = "VIEW_3D"
    bl_region_type = "TOOL_PROPS"
 
    def draw(self, context):
        self.layout.operator("matrices.export", text='Export')
 
#
#    The export button
#
class ExportMatrices(bpy.types.Operator):
    bl_idname = "matrices.export"
    bl_label = "Export Matrices"
    country = bpy.props.StringProperty()
 
    def execute(self, context):
        
        objects = bpy.context.scene.objects
        saveFile = open("c:/users/public/documents/newfile.cpp", "w")
        
        for obj in bpy.context.selected_objects:
            if obj.type == 'ARMATURE':
                saveFile.write("const uint32_t unJointCount = "+str(len(obj.pose.bones))+";\n")
                saveFile.write("const D3DXMATRIX asPose [] = {\n")
                for bone in obj.pose.bones:
                    saveFile.write("{")
                    for i in range(len(bone.matrix)):
                        fValue = bone.matrix[i]
                        for j in range(len(obj.matrix_world)):
                            saveFile.write(str(fValue[j])+"f, ")
                    saveFile.write("},\n")
                saveFile.write("};\n")
        
        saveFile.close()
        
        return{'FINISHED'}    
 
#
#	Registration
#
 
bpy.utils.register_module(__name__)