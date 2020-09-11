GLSLC=glslc
GLSLI=glsl_include

echo world.vert
cat world.vert|${GLSLI}|${GLSLC} -fshader-stage=vert -o world.vert.spv --target-env=vulkan1.1 -
echo tangent.vert
cat tangent.vert|${GLSLI}|${GLSLC} -fshader-stage=vert -o tangent.vert.spv --target-env=vulkan1.1 -


echo world.frag
cat world.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world.frag.spv --target-env=vulkan1.1 -
echo world_bc.frag
cat world_bc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc.frag.spv --target-env=vulkan1.1 -
echo world_mr.frag
cat world_mr.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr.frag.spv --target-env=vulkan1.1 -
echo world_bc_mr.frag
cat world_bc_mr.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr.frag.spv --target-env=vulkan1.1 -
echo world_bc_mr_oc.frag
cat world_bc_mr_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_oc.frag.spv --target-env=vulkan1.1 -
echo world_bc_oc.frag
cat world_bc_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_oc.frag.spv --target-env=vulkan1.1 -
echo world_mr_oc.frag
cat world_mr_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_oc.frag.spv --target-env=vulkan1.1 -
echo world_oc.frag
cat world_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_oc.frag.spv --target-env=vulkan1.1 -

echo world_em.frag
cat world_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_em.frag.spv --target-env=vulkan1.1 -
echo world_bc_em.frag
cat world_bc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_em.frag.spv --target-env=vulkan1.1 -
echo world_mr_em.frag
cat world_mr_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_em.frag.spv --target-env=vulkan1.1 -
echo world_bc_mr_em.frag
cat world_bc_mr_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_em.frag.spv --target-env=vulkan1.1 -
echo world_bc_mr_oc_em.frag
cat world_bc_mr_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_mr_oc_em.frag.spv --target-env=vulkan1.1 -
echo world_mc_oc_em.frag
cat world_bc_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_bc_oc_em.frag.spv --target-env=vulkan1.1 -
echo world_mr_oc_em.frag
cat world_mr_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_mr_oc_em.frag.spv --target-env=vulkan1.1 -
echo world_oc_em.frag
cat world_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o world_oc_em.frag.spv --target-env=vulkan1.1 -


echo tangent.frag
cat tangent.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent.frag.spv --target-env=vulkan1.1 -
echo tangent_bc.frag
cat tangent_bc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc.frag.spv --target-env=vulkan1.1 -
echo tangent_mr.frag
cat tangent_mr.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_mr.frag
cat tangent_bc_mr.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_mr_oc.frag
cat tangent_bc_mr_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_oc.frag
cat tangent_bc_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc.frag.spv --target-env=vulkan1.1 -
echo tangent_mr_oc.frag
cat tangent_mr_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc.frag.spv --target-env=vulkan1.1 -
echo tangent_oc.frag
cat tangent_oc.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc.frag.spv --target-env=vulkan1.1 -

echo tangent_em.frag
cat tangent_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_em.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_em.frag
cat tangent_bc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_em.frag.spv --target-env=vulkan1.1 -
echo tangent_mr_em.frag
cat tangent_mr_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_em.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_mr_em.frag
cat tangent_bc_mr_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_em.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_mr_oc_em.frag
cat tangent_bc_mr_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_em.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_oc_em.frag
cat tangent_bc_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_em.frag.spv --target-env=vulkan1.1 -
echo tangent_mr_oc_em.frag
cat tangent_mr_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_em.frag.spv --target-env=vulkan1.1 -
echo tangent_oc_em.frag
cat tangent_oc_em.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_em.frag.spv --target-env=vulkan1.1 -


echo tangent_no.frag
cat tangent_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_no.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_no.frag
cat tangent_bc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_no.frag.spv --target-env=vulkan1.1 -
echo tangent_mr_no.frag
cat tangent_mr_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_no.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_mr_no.frag
cat tangent_bc_mr_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_no.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_mr_oc_no.frag
cat tangent_bc_mr_oc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_no.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_oc_no.frag
cat tangent_bc_oc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_no.frag.spv --target-env=vulkan1.1 -
echo tangent_mr_oc_no.frag
cat tangent_mr_oc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_no.frag.spv --target-env=vulkan1.1 -
echo tangent_oc_no.frag
cat tangent_oc_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_no.frag.spv --target-env=vulkan1.1 -

echo tangent_em_no.frag
cat tangent_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_em_no.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_em_no.frag
cat tangent_bc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_em_no.frag.spv --target-env=vulkan1.1 -
echo tangent_mr_em_no.frag
cat tangent_mr_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_em_no.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_mr_em_no.frag
cat tangent_bc_mr_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_em_no.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_mr_oc_em_no.frag
cat tangent_bc_mr_oc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_mr_oc_em_no.frag.spv --target-env=vulkan1.1 -
echo tangent_bc_oc_em_no.frag
cat tangent_bc_oc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_bc_oc_em_no.frag.spv --target-env=vulkan1.1 -
echo tangent_mr_oc_em_no.frag
cat tangent_mr_oc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_mr_oc_em_no.frag.spv --target-env=vulkan1.1 -
echo tangent_oc_em_no.frag
cat tangent_oc_em_no.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o tangent_oc_em_no.frag.spv --target-env=vulkan1.1 -


echo special0.frag
cat special0.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special0.frag.spv --target-env=vulkan1.1 -
echo special1.frag
cat special1.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special1.frag.spv --target-env=vulkan1.1 -
echo special2.frag
cat special2.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special2.frag.spv --target-env=vulkan1.1 -
echo special3.frag
cat special3.frag|${GLSLI}|${GLSLC} -fshader-stage=frag -o special3.frag.spv --target-env=vulkan1.1 -

