# The Render System

Stage and Scene are the two top level constructs. A Stage is a single pass render, and a Scene contains the Models that get rendered.

Each Model in a Scene contains a VertexBufferObject, a Shader, and some Textures, along with a setable callback to set the Shader uniforms when the Model is rendered.

Scene also has a setable callback that is called on every Shader activation for Scene-wide uniforms such as the camera, lighting, etc.