add_library( vw SHARED
  vw/glfw.cpp
  vw/config.cpp
  vw/vma.cpp
  vw/image.cpp
  vw/buffer.cpp
  vw/list_device.cpp
  vw/is_capable.cpp
  vw/instance.cpp
  vw/context.cpp
  vw/render_pass.cpp
  vw/pipeline.cpp
  vw/framebuffer.cpp
  vw/shader.cpp
  vw/wait_for_idle.cpp
  vw/to_size.cpp
  vw/sampler.cpp
  vw/node.cpp
  vw/command_buffer.cpp
)
target_link_libraries(
  vw
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
  ${Boost_SYSTEM_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${Vulkan_LIBRARIES}
  ${OIIO_LIBRARIES}
)
add_library( viewer SHARED
  viewer/mesh.cpp
  viewer/buffer.cpp
  viewer/sampler.cpp
  viewer/image.cpp
  viewer/texture.cpp
  viewer/node.cpp
  viewer/document.cpp
  viewer/shader.cpp
)
target_link_libraries(
  viewer
  vw
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
  ${Boost_SYSTEM_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${Vulkan_LIBRARIES}
  ${OIIO_LIBRARIES}
)
add_executable( check_gltf check_gltf.cpp )
target_link_libraries( check_gltf
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${Vulkan_LIBRARIES}
  ${OIIO_LIBRARIES}
)
add_executable( draw draw.cpp )
target_link_libraries( draw
  viewer
  vw
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${Vulkan_LIBRARIES}
  ${OIIO_LIBRARIES}
)
add_executable( glsl_include glsl_include.cpp )
target_link_libraries( glsl_include
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)


