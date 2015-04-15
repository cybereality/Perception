	//This shader replacement currently does nothing, but is a placeholder for future development
    vs_3_0
    def c0, 2, -2, -1, 1
    def c52, 500, 500, 0, 0
    dcl_position v0
    dcl_color v1
    dcl_texcoord v2
    dcl_position o0
    dcl_color o1
    dcl_texcoord o2.xy
    add r0.xy, c52, v0
    mad o0.xy, r0, c0, c0.zwzw
    mov o0.zw, c0.w
    mov o1, v1
    mov o2.xy, v2
