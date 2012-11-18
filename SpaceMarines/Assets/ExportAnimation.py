import bpy
from struct import *

print("");

def isBoneAchildOfBoneB(boneA, boneB):
    if boneA.name in boneB.children:
        return True;
    for b in boneB.children:
        if isBoneAchildOfBoneB(boneA, b):
            return True;
    return False;    

def isBoneAAffectedByBoneB(obj, boneA, boneB):
    if boneA.use_deform == False: 
        return False;
    if boneA == boneB:
        return True;
    if isBoneAchildOfBoneB(boneA, boneB): #and obj.pose.bones[boneA.name].is_in_ik_chain:
        return True;

    return False;

for obj in bpy.data.objects:
    if obj.type != 'ARMATURE':
        pass;
    elif obj.animation_data.use_nla and len(obj.animation_data.nla_tracks) > 0:
        animData = obj.animation_data;
        armatureObj = obj;
        print("Armature: %s" % obj.name);
        armature = obj.data;
        
        #Loop through tracks
        for nlaTrack in animData.nla_tracks:
            if len(nlaTrack.strips) < 1:    
                continue;
            
            print("  NLA Tracks %s: " % nlaTrack.name);
            for nlaStrip in nlaTrack.strips:
                action = nlaStrip.action;
                print("\t" + action.name);
                file = open(action.name + ".anim", 'bw');
                
                #Write file header
                file.write(pack('=cccc', b'H', b'3', b'D', b'A'));
                file.write(pack('=I', 5));
                
                frameRange = [nlaStrip.frame_start, nlaStrip.frame_end];
                print("\t\tFrames: %i to %i" % (frameRange[0], frameRange[1]) );
                print("\t\tKeyframed groups:");
                
                keyFramedBones = [];
                for fCurveGroup in action.groups:
                    print("\t\t\tFCurve Group %s" % fCurveGroup.name);
                    for bone in armature.bones:                            
                        if bone.name == fCurveGroup.name:
                            keyFramedBones.append(bone);

                affectedBones = [];
                for boneA in armature.bones:
                    for boneB in keyFramedBones:
                        if isBoneAAffectedByBoneB(obj, boneA, boneB) and not(boneA in affectedBones):
                            affectedBones.append(obj.pose.bones[boneA.name]);
                
                file.write(pack('=I', len(affectedBones)));
                #Print list of affected bones
                print("\t\tAffected bones:");
                for affectedBone in affectedBones:
                    print("\t\t\t" + affectedBone.name);
                    file.write(pack('=%is' % len(affectedBone.name), bytes(affectedBone.name, 'utf-8')));
                    file.write(pack('B', 0)); #TODO Compression
                    for frame in range(int(frameRange[0]), int(frameRange[1] + 1)):
                        bpy.data.scenes[0].frame_set(frame);
                        m = affectedBone.matrix;
                        trans = m.decompose();
                        scale = trans[0];
                        rotation = trans[1];
                        scale = trans[2];
                        print("\t\t\t  f # %i : %f %f %f %f" % (frame - frameRange[0] + 1, trans[1].x, trans[1].y, trans[1].z, trans[1].w));
                            
                        
                #Writing goes here
                file.close();
        