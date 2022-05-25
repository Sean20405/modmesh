/*
 * Copyright (c) 2022, Yung-Yu Chen <yyc@solvcon.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <modmesh/view/common_detail.hpp> // Must be the first include.
#include <modmesh/view/RAxisMark.hpp>

#include <modmesh/modmesh.hpp>

#include <Qt>
#include <QWidget>
#include <Qt3DWindow>

#include <QByteArray>
#include <QGeometryRenderer>

#include <Qt3DCore/QBuffer>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QGeometry>
#include <Qt3DCore/QAttribute>
#include <Qt3DCore/QTransform>

#include <Qt3DExtras/QDiffuseSpecularMaterial>

namespace modmesh
{

RLine::RLine(QVector3D const & v0, QVector3D const & v1, QColor const & color, Qt3DCore::QNode * parent)
    : Qt3DCore::QEntity(parent)
    , m_geometry(new Qt3DCore::QGeometry(this))
    , m_renderer(new Qt3DRender::QGeometryRenderer())
    , m_material(new Qt3DExtras::QDiffuseSpecularMaterial())
{
    {
        auto * buf = new Qt3DCore::QBuffer(m_geometry);
        {
            QByteArray barray;
            barray.resize(3 * 2 * sizeof(float));
            float * ptr = reinterpret_cast<float *>(barray.data());
            ptr[0] = v0.x();
            ptr[1] = v0.y();
            ptr[2] = v0.z();
            ptr[3] = v1.x();
            ptr[4] = v1.y();
            ptr[5] = v1.z();
            buf->setData(barray);
        }

        {
            auto * vertices = new Qt3DCore::QAttribute(m_geometry);
            vertices->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
            vertices->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
            vertices->setVertexBaseType(Qt3DCore::QAttribute::Float);
            vertices->setVertexSize(3);
            vertices->setBuffer(buf);
            vertices->setByteStride(3 * sizeof(float));
            vertices->setCount(2);
            m_geometry->addAttribute(vertices);
        }
    }

    {
        auto * buf = new Qt3DCore::QBuffer(m_geometry);
        {
            QByteArray barray;
            barray.resize(2 * sizeof(uint32_t));
            auto * indices = reinterpret_cast<uint32_t *>(barray.data());
            indices[0] = 0;
            indices[1] = 1;
            buf->setData(barray);
        }

        {
            auto * indices = new Qt3DCore::QAttribute(m_geometry);
            indices->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
            indices->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
            indices->setBuffer(buf);
            indices->setCount(2);
            m_geometry->addAttribute(indices);
        }
    }

    m_renderer->setGeometry(m_geometry);
    m_renderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    addComponent(m_renderer);
    addComponent(m_material);
    m_material->setAmbient(color);
}

RAxisMark::RAxisMark(Qt3DCore::QNode * parent)
    : Qt3DCore::QEntity(parent)
    , m_xmark(new RLine(QVector3D(0, 0, 0), QVector3D(1, 0, 0), QColor(255, 0, 0, 255), this))
    , m_ymark(new RLine(QVector3D(0, 0, 0), QVector3D(0, 1, 0), QColor(0, 255, 0, 255), this))
    , m_zmark(new RLine(QVector3D(0, 0, 0), QVector3D(0, 0, 1), QColor(0, 0, 255, 255), this))
{
}

} /* end namespace modmesh */

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
