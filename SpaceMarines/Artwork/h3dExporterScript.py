import bpy

for obj in bpy.data.objects:
    if obj.type != 'ARMATURE':
        pass
    
    print("Object: " + obj.name)
    