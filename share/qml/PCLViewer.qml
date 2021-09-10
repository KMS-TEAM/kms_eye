import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQuick.Scene3D 2.0
import Qt3D.Core 2.0 as Q3D
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import pcl 1.0

Item {
    id: window
    width: 1200
    height: 800
    visible: true

    PointcloudReader {
        id: readerBunny
        filename: "/home/lacie/Github/data/kms_eye_output/pcl/result.pcd"
    }

    GridLayout {
        anchors.fill: parent
        Scene3D {
            id: scene3d
            Layout.minimumWidth: 50
            Layout.fillWidth: true
            Layout.fillHeight: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
            focus: true
            Q3D.Entity {
                id: sceneRoot

                Camera {
                    id: mainCamera
                    projectionType: CameraLens.PerspectiveProjection
                    fieldOfView: 75
                    aspectRatio: scene3d.width/scene3d.height
                    nearPlane : 0.1
                    farPlane : 1000.0
                    position: Qt.vector3d( 0.0, 0.0, -20.0 )
                    upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
                    viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
                }

                OrbitCameraController {
                    camera: mainCamera
                }

                components: [
                    RenderSettings {
                        activeFrameGraph: Viewport {
                            id: viewport
                            normalizedRect: Qt.rect(0.0, 0.0, 1.0, 1.0) // From Top Left
                            RenderSurfaceSelector {
                                CameraSelector {
                                    id : cameraSelector
                                    camera: mainCamera
                                    FrustumCulling {
                                        ClearBuffers {
                                            buffers : ClearBuffers.ColorDepthBuffer
                                            clearColor: "white"
                                            NoDraw {}
                                        }
                                        LayerFilter {
                                            layers: solidLayer
                                        }
                                        LayerFilter {
                                            layers: pointLayer
                                            RenderStateSet {
                                                renderStates: [
                                                    // If this is uncommented, following pointsizes are ignored in Qt5.7
                                                    //PointSize { sizeMode: PointSize.Fixed; value: 5.0 }, // exception when closing application in qt 5.7. Moreover PointSize
                                                    //PointSize { sizeMode: PointSize.Programmable }, //supported since OpenGL 3.2
                                                    DepthTest { depthFunction: DepthTest.Less }
                                                    //DepthMask { mask: true }
                                                ]
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    },
                    // Event Source will be set by the Qt3DQuickWindow
                    InputSettings {
                        eventSource: window
                        enabled: true
                    }
                ]

                PhongMaterial {
                    id: phongMaterial
                }

                TorusMesh {
                    id: torusMesh
                    radius: 5
                    minorRadius: 1
                    rings: 100
                    slices: 20
                }

                Q3D.Transform {
                    id: torusTransform
                    scale3D: Qt.vector3d(2.5, 2.5, 2.5)
                    //rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
                }

                Layer {
                    id: solidLayer
                }
                Layer {
                    id: pointLayer
                }

                Q3D.Entity {
                    id: pointcloud
                    property var meshTransform: Q3D.Transform {
                            id: pointcloudTransform
                            // property real userAngle: rotator.rotationAnimation
                            scale: 20
                            translation: Qt.vector3d(0, -2, 0)
                            // rotation: fromAxisAndAngle(Qt.vector3d(0, 1, 0), userAngle)
                        }
                    property GeometryRenderer pointcloudMesh: GeometryRenderer {
                            geometry: PointcloudGeometry { pointcloud: readerBunny.pointcloud }
                            primitiveType: GeometryRenderer.Points
                        }
                    property Material materialPoint: Material {
                        effect: Effect {
                            techniques: Technique {
                                renderPasses: RenderPass {
                                    shaderProgram: ShaderProgram {
                                        vertexShaderCode: loadSource("qrc:/shader/shader/pointcloud.vert")
                                        fragmentShaderCode: loadSource("qrc:/shader/shader/pointcloud.frag")
                                    }
                                }
                            }
                        }
                        parameters: Parameter { name: "pointSize"; value: 0.7 }
                    }
                    //property Material materialPoint: PerVertexColorMaterial {}
                    components: [ pointcloudMesh, materialPoint, meshTransform, pointLayer ]
                }
            }
        }
    }
//    NumberAnimation {
//        id: rotator
//        property real rotationAnimation
//        target: rotator
//        property: "rotationAnimation"
//        duration: 10000
//        from: -180
//        to: 180

//        loops: Animation.Infinite
//        running: true
//    }

    SystemPalette {
        id: palette
    }
}
