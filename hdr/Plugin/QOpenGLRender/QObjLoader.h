#ifndef QOBJLOADER_H
#define QOBJLOADER_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include <limits>

class QString;
class QIODevice;

struct FaceIndices;

class QObjLoader
{
public:
    QObjLoader();

    void setLoadTextureCoordinatesEnabled( bool b );
    bool isLoadTextureCoordinatesEnabled() const;

    void setMeshCenteringEnabled( bool b );
    bool isMeshCenteringEnabled() const;

    bool hasNormals() const;
    bool hasTextureCoordinates() const;

    bool load( const QString& fileName );
    bool load( QIODevice* ioDev );

    QVector<QVector3D> vertices() const;
    QVector<QVector3D> normals() const;
    QVector<QVector2D> textureCoordinates() const;
    QVector<unsigned int> indices() const;

private:
    void updateIndices(const QVector<QVector3D> &positions,
                       const QVector<QVector3D> &normals,
                       const QVector<QVector2D> &texCoords,
                       const QHash<FaceIndices, unsigned int> &faceIndexMap,
                       const QVector<FaceIndices> &faceIndexVector);

    void generateAveragedNormals(const QVector<QVector3D>& points,
                                 QVector<QVector3D>& normals,
                                 const QVector<unsigned int>& faces) const;

    void center(QVector<QVector3D>& points);

    bool m_loadTextureCoords;
    bool m_generateTangents;
    bool m_centerMesh;

    QVector<QVector3D> m_points;
    QVector<QVector3D> m_normals;
    QVector<QVector2D> m_texCoords;
    QVector<unsigned int> m_indices;
};

#endif // QOBJLOADER_H
