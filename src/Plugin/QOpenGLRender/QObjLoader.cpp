#include "QOpenGLRender/QObjLoader.h"
#include "QOpenGLRender/QAxisAlignedBoundingBox.h"

#include "AppConstant.h"

#include <QFile>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QTextStream>
#include <QVector>
#include <QHash>


struct FaceIndices
{
    FaceIndices()
        : positionIndex(std::numeric_limits<unsigned int>::max())
        , texCoordIndex(std::numeric_limits<unsigned int>::max())
        , normalIndex(std::numeric_limits<unsigned int>::max())
    {

    }

    FaceIndices(unsigned int posIndex, unsigned int tcIndex, unsigned int nIndex)
        : positionIndex(posIndex)
        , texCoordIndex(tcIndex)
        , normalIndex(nIndex)
    {}

    bool operator == (const FaceIndices &other) const
    {
        return positionIndex == other.positionIndex &&
                texCoordIndex == other.texCoordIndex &&
                normalIndex == other.normalIndex;
    }

    unsigned int positionIndex;
    unsigned int texCoordIndex;
    unsigned int normalIndex;
};

inline uint qHash(const FaceIndices &faceIndices, uint seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(faceIndices.positionIndex, seed);
    seed = hash(faceIndices.normalIndex, seed);
    seed = hash(faceIndices.texCoordIndex, seed);
    return seed;
}

static void addFaceVertex( const FaceIndices& faceIndices,
                           QVector<FaceIndices>& faceIndexVector,
                           QHash<FaceIndices, unsigned int>& faceIndexMap )
{
    if (faceIndices.positionIndex != std::numeric_limits<unsigned int>::max()) {
        faceIndexVector.append(faceIndices);
        if (!faceIndexMap.contains(faceIndices))
            faceIndexMap.insert(faceIndices, faceIndexMap.size());
    } else {
        qWarning( "Missing position index" );
    }
}

QObjLoader::QObjLoader()
    : m_loadTextureCoords(true)
    , m_centerMesh(false)
{

}

void QObjLoader::setLoadTextureCoordinatesEnabled(bool b)
{
    m_loadTextureCoords = b;
}

bool QObjLoader::isLoadTextureCoordinatesEnabled() const
{
    return m_loadTextureCoords;
}

void QObjLoader::setMeshCenteringEnabled(bool b)
{
    m_centerMesh = b;
}

bool QObjLoader::isMeshCenteringEnabled() const
{
    return m_centerMesh;
}

bool QObjLoader::hasNormals() const
{
    return !m_normals.isEmpty();
}

bool QObjLoader::hasTextureCoordinates() const
{
    return !m_texCoords.isEmpty();
}

bool QObjLoader::load(const QString &fileName)
{
    QFile file( fileName );
    if ( !file.open( ::QIODevice::ReadOnly | ::QIODevice::Text ) )
    {
        qDebug() << "Could not open file" << fileName << "for reading";
        return false;
    }

    return load( &file );
}

bool QObjLoader::load(QIODevice *ioDev)
{
    Q_CHECK_PTR(ioDev);
    if (!ioDev->isOpen()) {
        qWarning() << "iodevice" << ioDev << "not open for reading";
        return false;
    }

    int faceCount = 0;

    // Parse faces taking into account each vertex in a face can index different indices
    // for the positions, normals and texture coords;
    // Generate unique vertices (in OpenGL parlance) and output to m_points, m_texCoords,
    // m_normals and calculate mapping from faces to unique indices
    QVector<QVector3D> positions;
    QVector<QVector3D> normals;
    QVector<QVector2D> texCoords;
    QHash<FaceIndices, unsigned int> faceIndexMap;
    QVector<FaceIndices> faceIndexVector;

    QTextStream stream(ioDev);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        line = line.simplified();

        if (line.length() > 0 && line.at(0) != QChar::fromLatin1('#')) {
            QTextStream lineStream(&line, QIODevice::ReadOnly);
            QString token;
            lineStream >> token;

            if (token == QStringLiteral("v")) {
                float x, y, z;
                lineStream >> x >> y >> z;
                positions.append(QVector3D( x, y, z ));
            } else if (token == QStringLiteral("vt") && m_loadTextureCoords) {
                // Process texture coordinate
                float s,t;
                lineStream >> s >> t;
                texCoords.append(QVector2D(s, t));
            } else if (token == QStringLiteral("vn")) {
                float x, y, z;
                lineStream >> x >> y >> z;
                normals.append(QVector3D( x, y, z ));
            } else if (token == QStringLiteral("f")) {
                // Process face
                ++faceCount;
                QVector<FaceIndices> face;
                int faceVertices = 0;
                while (!lineStream.atEnd()) {
                    QString faceString;
                    lineStream >> faceString;

                    FaceIndices faceIndices;
                    QStringList indices = faceString.split(QChar::fromLatin1('/'));
                    switch (indices.size()) {
                    case 3:
                        faceIndices.normalIndex = indices.at(2).toInt() - 1;  // fall through
                    case 2:
                        faceIndices.texCoordIndex = indices.at(1).toInt() - 1; // fall through
                    case 1:
                        faceIndices.positionIndex = indices.at(0).toInt() - 1;
                        break;
                    default:
                        qWarning() << "Unsupported number of indices in face element";
                    }

                    face.append(faceIndices);
                    ++faceVertices;
                }

                // If number of edges in face is greater than 3,
                // decompose into triangles as a triangle fan.
                FaceIndices v0 = face[0];
                FaceIndices v1 = face[1];
                FaceIndices v2 = face[2];

                // First face
                addFaceVertex(v0, faceIndexVector, faceIndexMap);
                addFaceVertex(v1, faceIndexVector, faceIndexMap);
                addFaceVertex(v2, faceIndexVector, faceIndexMap);

                for ( int i = 3; i < face.size(); ++i ) {
                    v1 = v2;
                    v2 = face[i];
                    addFaceVertex(v0, faceIndexVector, faceIndexMap);
                    addFaceVertex(v1, faceIndexVector, faceIndexMap);
                    addFaceVertex(v2, faceIndexVector, faceIndexMap);
                }
            } // end of face
        } // end of input line
    } // while (!stream.atEnd())

    updateIndices(positions, normals, texCoords, faceIndexMap, faceIndexVector);

    if (m_normals.isEmpty())
        generateAveragedNormals(m_points, m_normals, m_indices);

    if (m_centerMesh)
        center(m_points);

    qDebug() << "Loaded mesh:";
    qDebug() << " " << m_points.size() << "points";
    qDebug() << " " << faceCount << "faces";
    qDebug() << " " << m_indices.size() / 3 << "triangles.";
    qDebug() << " " << m_normals.size() << "normals";
    qDebug() << " " << m_texCoords.size() << "texture coordinates.";

    return true;
}

QVector<QVector3D> QObjLoader::vertices() const
{
    return m_points;
}

QVector<QVector3D> QObjLoader::normals() const
{
    return m_normals;
}

QVector<QVector2D> QObjLoader::textureCoordinates() const
{
    return m_texCoords;
}

QVector<unsigned int> QObjLoader::indices() const
{
    return m_indices;
}

void QObjLoader::updateIndices(const QVector<QVector3D> &positions,
                               const QVector<QVector3D> &normals,
                               const QVector<QVector2D> &texCoords,
                               const QHash<FaceIndices, unsigned int> &faceIndexMap,
                               const QVector<FaceIndices> &faceIndexVector)
{
    // Iterate over the faceIndexMap and pull out pos, texCoord and normal data
    // thereby generating unique vertices of data (by OpenGL definition)
    const int vertexCount = faceIndexMap.size();
    const bool hasTexCoords = !texCoords.isEmpty();
    const bool hasNormals = !normals.isEmpty();

    m_points.resize(vertexCount);
    m_texCoords.clear();
    if (hasTexCoords)
        m_texCoords.resize(vertexCount);
    m_normals.clear();
    if (hasNormals)
        m_normals.resize(vertexCount);

    foreach (const FaceIndices &faceIndices, faceIndexMap.keys()) {
        const int i = faceIndexMap.value(faceIndices);

        m_points[i] = positions[faceIndices.positionIndex];
        if (hasTexCoords)
            m_texCoords[i] = texCoords[faceIndices.texCoordIndex];
        if (hasNormals)
            m_normals[i] = normals[faceIndices.normalIndex];
    }

    // Now iterate over the face indices and lookup the unique vertex index
    const int indexCount = faceIndexVector.size();
    m_indices.clear();
    m_indices.reserve(indexCount);
    foreach (const FaceIndices &faceIndices, faceIndexVector) {
        const unsigned int i = faceIndexMap.value(faceIndices);
        m_indices.append(i);
    }
}

void QObjLoader::generateAveragedNormals(const QVector<QVector3D> &points,
                                         QVector<QVector3D> &normals,
                                         const QVector<unsigned int> &faces) const
{
    for ( int i = 0; i < points.size(); ++i )
        normals.append( QVector3D() );

    for ( int i = 0; i < faces.size(); i += 3 )
    {
        const QVector3D& p1 = points[ faces[i]   ];
        const QVector3D& p2 = points[ faces[i+1] ];
        const QVector3D& p3 = points[ faces[i+2] ];

        QVector3D a = p2 - p1;
        QVector3D b = p3 - p1;
        QVector3D n = QVector3D::crossProduct( a, b ).normalized();

        normals[ faces[i]   ] += n;
        normals[ faces[i+1] ] += n;
        normals[ faces[i+2] ] += n;
    }

    for ( int i = 0; i < normals.size(); ++i )
        normals[i].normalize();
}

void QObjLoader::center(QVector<QVector3D> &points)
{
    if ( points.isEmpty() )
        return;

    QAxisAlignedBoundingBox bb(points);
    QVector3D center = bb.center();

    // Translate center of the AABB to the origin
    for ( int i = 0; i < points.size(); ++i )
    {
        QVector3D& point = points[i];
        point = point - center;
    }
}


