/* 
 * File:   QMeshInteractor.cpp
 * Author: b4n92uid
 * 
 * Created on 29 août 2011, 07:23
 */

#include "QMeshInteractor.h"
#include "MainWindow.h"

QMeshInteractor::QMeshInteractor(MainWindow* mainwin, tbe::scene::Mesh* target)
: QNodeInteractor(mainwin, target), m_target(target)
{
}

QMeshInteractor::~QMeshInteractor()
{
}

QString QMeshInteractor::typeName() const
{
    return "Mesh";
}

tbe::scene::Material* QMeshInteractor::getSelectedMaterial()
{
    using namespace tbe::scene;

    QModelIndex index = m_mainwin->nodesGui.mesh.matedit->materialsView->currentIndex();

    if(index.isValid())
        return m_mainwin->nodesGui.mesh.matedit->materialsModel
            ->itemFromIndex(index)->data().value<Material*>();
    else
        return NULL;
}

void QMeshInteractor::setIncludedMaterial(bool state)
{
    m_mainwin->tbeWidget()->sceneParser()->setIncludedMaterialFile(m_target, state);

    if(state)
        delMaterialFile();
    else
        updateGui();
}

void QMeshInteractor::openMaterialFile()
{
    QString filepath = QFileDialog::getOpenFileName(m_mainwin);

    if(!filepath.isEmpty())
    {
        m_mainwin->tbeWidget()->sceneParser()->setMaterialFile(m_target, filepath.toStdString());
        m_mainwin->tbeWidget()->sceneParser()->reloadMaterialFiles();

        m_mainwin->nodesGui.mesh.matedit->materialInfo->setText(filepath);

        updateGui();
    }
}

void QMeshInteractor::saveMaterialFile()
{
    if(m_mainwin->nodesGui.mesh.includedmat->isChecked())
    {
        m_mainwin->nodesGui.mesh.matedit->hide();
        return;
    }

    else if(m_mainwin->tbeWidget()->sceneParser()->getMaterialFile(m_target).empty())
    {
        QString filepath = QFileDialog::getSaveFileName(m_mainwin);

        if(!filepath.isEmpty())
        {
            m_mainwin->tbeWidget()->sceneParser()->setMaterialFile(m_target, filepath.toStdString());
            m_mainwin->tbeWidget()->sceneParser()->saveMaterialFile(m_target);
            m_mainwin->tbeWidget()->sceneParser()->reloadMaterialFiles();

            m_mainwin->nodesGui.mesh.matedit->hide();
        }
    }

    else
    {
        m_mainwin->tbeWidget()->sceneParser()->saveMaterialFile(m_target);
        m_mainwin->tbeWidget()->sceneParser()->reloadMaterialFiles();

        m_mainwin->nodesGui.mesh.matedit->hide();
    }
}

void QMeshInteractor::delMaterialFile()
{
    m_mainwin->tbeWidget()->sceneParser()->deleteMaterialFile(m_target);

    updateGui();
}

void QMeshInteractor::materialSelected(const QModelIndex& index)
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    // Reload textures list
    m_mainwin->nodesGui.mesh.matedit->textured->setChecked(mat->isEnable(Material::TEXTURED));

    m_mainwin->nodesGui.mesh.matedit->textureModel->
            removeRows(0, m_mainwin->nodesGui.mesh.matedit->textureModel->rowCount());

    unsigned count = mat->getTexturesCount();
    for(unsigned i = 0; i < count; i++)
    {
        Texture tex = mat->getTexture(i);

        QString path = QString::fromStdString(tex.getFilename());

        QVariant data;
        data.setValue(tex);

        QStandardItem* item = new QStandardItem(QFileInfo(path).baseName());
        item->setData(data);

        m_mainwin->nodesGui.mesh.matedit->textureModel->appendRow(item);
    }

    // Select the first texture unit and update GUI
    QModelIndex first = m_mainwin->nodesGui.mesh.matedit->textureModel->index(0, 0);

    m_mainwin->nodesGui.mesh.matedit->textureView->setCurrentIndex(first);
    textureSelected(first);

    // Update material gui flag
    m_mainwin->nodesGui.mesh.matedit->foged->setChecked(mat->isEnable(Material::FOGED));
    m_mainwin->nodesGui.mesh.matedit->lighted->setChecked(mat->isEnable(Material::LIGHTED));
    m_mainwin->nodesGui.mesh.matedit->culltrick->setChecked(mat->isEnable(Material::VERTEX_SORT_CULL_TRICK));

    // Update material colors and lighting value
    m_mainwin->nodesGui.mesh.matedit->rgb->setValue(tbe::math::vec43(mat->getColor()));
    m_mainwin->nodesGui.mesh.matedit->ambiant->setValue(tbe::math::vec43(mat->getAmbient()));
    m_mainwin->nodesGui.mesh.matedit->diffuse->setValue(tbe::math::vec43(mat->getDiffuse()));
    m_mainwin->nodesGui.mesh.matedit->specular->setValue(tbe::math::vec43(mat->getSpecular()));

    // Update material blending state
    bool blending = mat->isEnable(Material::BLEND_MOD)
            || mat->isEnable(Material::BLEND_ADD)
            || mat->isEnable(Material::BLEND_MUL);

    m_mainwin->nodesGui.mesh.matedit->blending->setChecked(blending);

    if(blending)
    {
        if(mat->isEnable(Material::BLEND_MOD))
            m_mainwin->nodesGui.mesh.matedit->blend_modulate->setChecked(true);

        else if(mat->isEnable(Material::BLEND_ADD))
            m_mainwin->nodesGui.mesh.matedit->blend_additive->setChecked(true);

        else if(mat->isEnable(Material::BLEND_MUL))
            m_mainwin->nodesGui.mesh.matedit->blend_mul->setChecked(true);
    }

    else
        m_mainwin->nodesGui.mesh.matedit->blend_modulate->setChecked(true);


    // Alpha
    bool alpha = mat->isEnable(Material::ALPHA);
    m_mainwin->nodesGui.mesh.matedit->alpha->setChecked(alpha);
    m_mainwin->nodesGui.mesh.matedit->alphathreshold->setValue(mat->getAlphaThershold());
}

void QMeshInteractor::setFoged(bool stat)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
        mat->enable(Material::FOGED);
    else
        mat->disable(Material::FOGED);
}

void QMeshInteractor::setTextured(bool stat)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
        mat->enable(Material::TEXTURED);
    else
        mat->disable(Material::TEXTURED);
}

void QMeshInteractor::textureSelected(const QModelIndex& index)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    // Texture's blending
    if(mat->getTextureBlend(index.row()) == Material::MODULATE)
        m_mainwin->nodesGui.mesh.matedit->texture_modulate->setChecked(true);

    else if(mat->getTextureBlend(index.row()) == Material::ADDITIVE)
        m_mainwin->nodesGui.mesh.matedit->texture_additive->setChecked(true);

    else if(mat->getTextureBlend(index.row()) == Material::REPLACE)
        m_mainwin->nodesGui.mesh.matedit->texture_replace->setChecked(true);

    // Clipping & animation
    m_mainwin->nodesGui.mesh.matedit->cliping->setChecked(mat->isTextureClipped(index.row()));
    m_mainwin->nodesGui.mesh.matedit->clipping_animation->setChecked(mat->getTextureAnimation(index.row()) > 0);
    m_mainwin->nodesGui.mesh.matedit->clipping_animation_msec->setValue(mat->getTextureAnimation(index.row()));
    m_mainwin->nodesGui.mesh.matedit->clipping_framesize->setValue(mat->getTextureFrameSize(index.row()));
    m_mainwin->nodesGui.mesh.matedit->clipping_part->setValue(mat->getTexturePart(index.row()));
}

void QMeshInteractor::addTexture()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    QStringList paths = QFileDialog::getOpenFileNames(m_mainwin, QString(),
                                                      m_mainwin->m_workingDir.meshTexture);

    int offset = mat->getTexturesCount();

    for(int i = 0; i < paths.size(); i++)
    {
        try
        {
            Texture tex(paths[i].toStdString(), true);

            QVariant data;
            data.setValue(tex);

            QStandardItem* item = new QStandardItem(QFileInfo(paths[i]).baseName());
            item->setData(data);

            m_mainwin->nodesGui.mesh.matedit->textureModel->appendRow(item);
            mat->setTexture(tex, offset + i);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(m_mainwin, "Erreur de chargement", e.what());
        }
    }
}

void QMeshInteractor::delTexture()
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    QModelIndex index = m_mainwin->nodesGui.mesh.matedit->textureView->currentIndex();
    mat->dropTexture(index.row());

    m_mainwin->nodesGui.mesh.matedit->textureModel->removeRow(index.row());
}

void QMeshInteractor::textureUp()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    int srcindex = m_mainwin->nodesGui.mesh.matedit->textureView->currentIndex().row();

    if(srcindex > 0)
    {
        int dstindex = srcindex - 1;

        Material* mat = getSelectedMaterial();

        Texture src = mat->getTexture(srcindex);
        Texture dst = mat->getTexture(dstindex);

        mat->setTexture(src, dstindex);
        mat->setTexture(dst, srcindex);

        QList<QStandardItem*> items = m_mainwin->nodesGui.mesh.matedit->textureModel->takeRow(srcindex);
        m_mainwin->nodesGui.mesh.matedit->textureModel->insertRow(dstindex, items);

        m_mainwin->nodesGui.mesh.matedit->textureView
                ->setCurrentIndex(m_mainwin->nodesGui.mesh.matedit->textureModel->index(dstindex, 0));
    }
}

void QMeshInteractor::textureDown()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    int srcindex = m_mainwin->nodesGui.mesh.matedit->textureView->currentIndex().row();

    if(srcindex < m_mainwin->nodesGui.mesh.matedit->textureModel->rowCount() - 1)
    {
        int dstindex = srcindex + 1;

        Material* mat = getSelectedMaterial();

        Texture src = mat->getTexture(srcindex);
        Texture dst = mat->getTexture(dstindex);

        mat->setTexture(src, dstindex);
        mat->setTexture(dst, srcindex);

        QList<QStandardItem*> items = m_mainwin->nodesGui.mesh.matedit->textureModel->takeRow(srcindex);
        m_mainwin->nodesGui.mesh.matedit->textureModel->insertRow(dstindex, items);

        m_mainwin->nodesGui.mesh.matedit->textureView
                ->setCurrentIndex(m_mainwin->nodesGui.mesh.matedit->textureModel->index(dstindex, 0));
    }
}

void QMeshInteractor::textureSetBlendMode()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    QModelIndex index = m_mainwin->nodesGui.mesh.matedit->textureView->currentIndex();

    Material* mat = getSelectedMaterial();

    if(m_mainwin->nodesGui.mesh.matedit->texture_modulate->isChecked())
        mat->setTextureBlend(Material::MODULATE, index.row());

    else if(m_mainwin->nodesGui.mesh.matedit->texture_additive->isChecked())
        mat->setTextureBlend(Material::ADDITIVE, index.row());

    else if(m_mainwin->nodesGui.mesh.matedit->texture_replace->isChecked())
        mat->setTextureBlend(Material::REPLACE, index.row());
}

void QMeshInteractor::setBlend(bool stat)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
    {
        mat->enable(Material::COLORED);

        materialSetBlendMode();
    }

    else
    {
        mat->disable(Material::BLEND_ADD | Material::BLEND_MOD
                     | Material::BLEND_MUL | Material::COLORED);
    }
}

void QMeshInteractor::materialSetBlendMode()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    Material* mat = getSelectedMaterial();

    mat->disable(Material::BLEND_MOD | Material::BLEND_ADD | Material::BLEND_MUL);

    if(m_mainwin->nodesGui.mesh.matedit->blend_modulate->isChecked())
        mat->enable(Material::BLEND_MOD);

    else if(m_mainwin->nodesGui.mesh.matedit->blend_additive->isChecked())
        mat->enable(Material::BLEND_ADD);

    else if(m_mainwin->nodesGui.mesh.matedit->blend_mul->isChecked())
        mat->enable(Material::BLEND_MUL);
}

void QMeshInteractor::setBillBoard()
{
    tbe::Vector2b apply;
    apply.x = m_mainwin->nodesGui.mesh.billboardX->isChecked();
    apply.y = m_mainwin->nodesGui.mesh.billboardY->isChecked();

    m_target->setBillBoard(apply);
}

void QMeshInteractor::setColor(const tbe::Vector3f& value)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    tbe::Vector4f color = tbe::math::vec34(value);
    color.w = m_mainwin->nodesGui.mesh.matedit->color_a->value();

    mat->setColor(color);
}

void QMeshInteractor::setOpacity(double value)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setOpacity((float)value);
}

void QMeshInteractor::setAmbiant(tbe::Vector3f color)
{
    getSelectedMaterial()->setAmbient(tbe::math::vec34(color));
}

void QMeshInteractor::setDiffuse(tbe::Vector3f color)
{
    getSelectedMaterial()->setDiffuse(tbe::math::vec34(color));
}

void QMeshInteractor::setSpecular(tbe::Vector3f color)
{
    getSelectedMaterial()->setSpecular(tbe::math::vec34(color));
}

void QMeshInteractor::setTextureClipping(bool state)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setTextureClipped(state);
}

void QMeshInteractor::setTextureFrameSize(const tbe::Vector2i& size)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setTextureFrameSize(size);
}

void QMeshInteractor::setTexturePart(const tbe::Vector2i& value)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setTexturePart(value);
}

void QMeshInteractor::setFrameAnimation(bool state)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();
    mat->setTextureAnimation(state ? 500 : 0);
}

void QMeshInteractor::setFrameDuration(int value)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();
    mat->setTextureAnimation(value);
}

void QMeshInteractor::setAlpha(bool stat)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
        mat->enable(Material::ALPHA);
    else
        mat->disable(Material::ALPHA);
}

void QMeshInteractor::setAlphaThreshold(double value)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setAlphaThershold((float)value);
}

void QMeshInteractor::setCullTrick(bool stat)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
        mat->enable(Material::VERTEX_SORT_CULL_TRICK);
    else
        mat->disable(Material::VERTEX_SORT_CULL_TRICK);
}

void QMeshInteractor::setLighted(bool stat)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
        mat->enable(Material::LIGHTED);
    else
        mat->disable(Material::LIGHTED);
}

void QMeshInteractor::pastMaterials()
{
    using namespace tbe::scene;

    if(!m_mainwin->nodesGui.mesh.matedit->source_mesh)
        return;

    Mesh* src = m_mainwin->nodesGui.mesh.matedit->source_mesh->m_target;

    tbe::Matrix4 mat = m_target->getMatrix();

    *m_target = *src;

    m_target->setMatrix(mat);

    updateGui();
}

void QMeshInteractor::copyMaterials()
{
    m_mainwin->nodesGui.mesh.matedit->source_mesh = this;
}

void QMeshInteractor::bindWithGui()
{
    QNodeInteractor::bindWithGui();

    connect(m_mainwin->nodesGui.mesh.matedit->ok, SIGNAL(clicked()), this, SLOT(saveMaterialFile()));

    connect(m_mainwin->nodesGui.mesh.editmatfile, SIGNAL(clicked()), m_mainwin->nodesGui.mesh.matedit, SLOT(show()));
    connect(m_mainwin->nodesGui.mesh.openmatfile, SIGNAL(clicked()), this, SLOT(openMaterialFile()));
    connect(m_mainwin->nodesGui.mesh.delmatfile, SIGNAL(clicked()), this, SLOT(delMaterialFile()));

    connect(m_mainwin->nodesGui.mesh.includedmat, SIGNAL(clicked(bool)), this, SLOT(setIncludedMaterial(bool)));

    connect(m_mainwin->nodesGui.mesh.matedit->textured, SIGNAL(clicked(bool)), this, SLOT(setTextured(bool)));
    connect(m_mainwin->nodesGui.mesh.matedit->lighted, SIGNAL(clicked(bool)), this, SLOT(setLighted(bool)));
    connect(m_mainwin->nodesGui.mesh.matedit->foged, SIGNAL(clicked(bool)), this, SLOT(setFoged(bool)));
    connect(m_mainwin->nodesGui.mesh.matedit->alpha, SIGNAL(clicked(bool)), this, SLOT(setAlpha(bool)));
    connect(m_mainwin->nodesGui.mesh.matedit->blending, SIGNAL(clicked(bool)), this, SLOT(setBlend(bool)));
    connect(m_mainwin->nodesGui.mesh.matedit->culltrick, SIGNAL(clicked(bool)), this, SLOT(setCullTrick(bool)));
    connect(m_mainwin->nodesGui.mesh.matedit->rgb, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setColor(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.mesh.matedit->color_a, SIGNAL(valueChanged(double)), this, SLOT(setOpacity(double)));

    connect(m_mainwin->nodesGui.mesh.matedit->ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setAmbiant(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.mesh.matedit->diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setDiffuse(const tbe::Vector3f&)));
    connect(m_mainwin->nodesGui.mesh.matedit->specular, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setSpecular(const tbe::Vector3f&)));

    connect(m_mainwin->nodesGui.mesh.billboardX, SIGNAL(clicked()), this, SLOT(setBillBoard()));
    connect(m_mainwin->nodesGui.mesh.billboardY, SIGNAL(clicked()), this, SLOT(setBillBoard()));

    connect(m_mainwin->nodesGui.mesh.matedit->alphathreshold, SIGNAL(valueChanged(double)), this, SLOT(setAlphaThreshold(double)));

    connect(m_mainwin->nodesGui.mesh.matedit->add, SIGNAL(clicked()), this, SLOT(addTexture()));
    connect(m_mainwin->nodesGui.mesh.matedit->del, SIGNAL(clicked()), this, SLOT(delTexture()));
    connect(m_mainwin->nodesGui.mesh.matedit->up, SIGNAL(clicked()), this, SLOT(textureUp()));
    connect(m_mainwin->nodesGui.mesh.matedit->down, SIGNAL(clicked()), this, SLOT(textureDown()));

    connect(m_mainwin->nodesGui.mesh.matedit->blend_additive, SIGNAL(clicked()), this, SLOT(materialSetBlendMode()));
    connect(m_mainwin->nodesGui.mesh.matedit->blend_modulate, SIGNAL(clicked()), this, SLOT(materialSetBlendMode()));
    connect(m_mainwin->nodesGui.mesh.matedit->blend_mul, SIGNAL(clicked()), this, SLOT(materialSetBlendMode()));

    connect(m_mainwin->nodesGui.mesh.matedit->texture_additive, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));
    connect(m_mainwin->nodesGui.mesh.matedit->texture_modulate, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));
    connect(m_mainwin->nodesGui.mesh.matedit->texture_replace, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));

    connect(m_mainwin->nodesGui.mesh.matedit->cliping, SIGNAL(clicked(bool)), this, SLOT(setTextureClipping(bool)));
    connect(m_mainwin->nodesGui.mesh.matedit->clipping_animation, SIGNAL(clicked(bool)), this, SLOT(setFrameAnimation(bool)));
    connect(m_mainwin->nodesGui.mesh.matedit->clipping_framesize, SIGNAL(valueChanged(const tbe::Vector2i&)), this, SLOT(setTextureFrameSize(const tbe::Vector2i&)));
    connect(m_mainwin->nodesGui.mesh.matedit->clipping_part, SIGNAL(valueChanged(const tbe::Vector2i&)), this, SLOT(setTexturePart(const tbe::Vector2i&)));
    connect(m_mainwin->nodesGui.mesh.matedit->clipping_animation_msec, SIGNAL(valueChanged(int)), this, SLOT(setFrameDuration(int)));

    connect(m_mainwin->nodesGui.mesh.matedit->copy, SIGNAL(clicked()), this, SLOT(copyMaterials()));
    connect(m_mainwin->nodesGui.mesh.matedit->past, SIGNAL(clicked()), this, SLOT(pastMaterials()));

    connect(m_mainwin->nodesGui.mesh.matedit->textureView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(textureSelected(const QModelIndex &)));

    connect(m_mainwin->nodesGui.mesh.matedit->materialsView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(materialSelected(const QModelIndex &)));

    updateGui();

    m_mainwin->nodesGui.attribTab->setCurrentIndex(0);
}

void QMeshInteractor::unbindFromGui()
{
    QNodeInteractor::unbindFromGui();

    disconnect(m_mainwin->nodesGui.mesh.matedit->ok, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.editmatfile, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.openmatfile, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.delmatfile, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.includedmat, SIGNAL(clicked(bool)), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->textured, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->lighted, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->foged, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->alpha, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->blending, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->culltrick, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->color_a, SIGNAL(valueChanged(double)), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.billboardX, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.billboardY, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->alphathreshold, SIGNAL(valueChanged(double)), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->add, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->del, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->up, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->down, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->blend_additive, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->blend_modulate, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->blend_mul, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->texture_additive, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->texture_modulate, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->texture_replace, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->cliping, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->clipping_animation, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->clipping_framesize, SIGNAL(valueChanged(const tbe::Vector2i&)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->clipping_part, SIGNAL(valueChanged(const tbe::Vector2i&)), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->clipping_animation_msec, SIGNAL(valueChanged(int)), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->copy, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.mesh.matedit->past, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->textureView, SIGNAL(clicked(const QModelIndex &)), 0, 0);

    disconnect(m_mainwin->nodesGui.mesh.matedit->materialsView, SIGNAL(clicked(const QModelIndex &)), 0, 0);

    m_mainwin->nodesGui.mesh.matedit->materialsModel
            ->removeRows(0, m_mainwin->nodesGui.mesh.matedit->materialsModel->rowCount());

    m_mainwin->nodesGui.mesh.includedmat->setChecked(false);

    m_mainwin->nodesGui.mesh.materialFilePath->clear();

    m_mainwin->nodesGui.mesh.billboardX->setChecked(false);
    m_mainwin->nodesGui.mesh.billboardY->setChecked(false);
}

void QMeshInteractor::updateGui()
{
    using namespace tbe::scene;

    QNodeInteractor::updateGui();

    QSignalBlocker blocker;
    blocker
            << m_mainwin->nodesGui.mesh.add
            << m_mainwin->nodesGui.mesh.billboardX
            << m_mainwin->nodesGui.mesh.billboardY
            << m_mainwin->nodesGui.mesh.delmatfile
            << m_mainwin->nodesGui.mesh.editmatfile
            << m_mainwin->nodesGui.mesh.includedmat
            << m_mainwin->nodesGui.mesh.materialFilePath
            << m_mainwin->nodesGui.mesh.openmatfile

            << m_mainwin->nodesGui.mesh.matedit->alpha
            << m_mainwin->nodesGui.mesh.matedit->alphathreshold
            << m_mainwin->nodesGui.mesh.matedit->ambiant
            << m_mainwin->nodesGui.mesh.matedit->blend_additive
            << m_mainwin->nodesGui.mesh.matedit->blend_modulate
            << m_mainwin->nodesGui.mesh.matedit->blend_mul
            << m_mainwin->nodesGui.mesh.matedit->blending
            << m_mainwin->nodesGui.mesh.matedit->cliping
            << m_mainwin->nodesGui.mesh.matedit->clipping_animation
            << m_mainwin->nodesGui.mesh.matedit->clipping_animation_msec
            << m_mainwin->nodesGui.mesh.matedit->clipping_framesize
            << m_mainwin->nodesGui.mesh.matedit->clipping_part
            << m_mainwin->nodesGui.mesh.matedit->culltrick
            << m_mainwin->nodesGui.mesh.matedit->diffuse
            << m_mainwin->nodesGui.mesh.matedit->foged
            << m_mainwin->nodesGui.mesh.matedit->rgb
            << m_mainwin->nodesGui.mesh.matedit->specular;

    blocker.block();

    // Reload materials list
    m_mainwin->nodesGui.mesh.matedit->materialsModel->
            removeRows(0, m_mainwin->nodesGui.mesh.matedit->materialsModel->rowCount());

    Material::Array matarr = m_target->getAllMaterial();

    foreach(Material* mat, matarr)
    {
        QVariant data;
        data.setValue<Material*>(mat);

        QStandardItem* item = new QStandardItem(QString::fromStdString(mat->getName()));
        item->setData(data);

        m_mainwin->nodesGui.mesh.matedit->materialsModel->appendRow(item);
    }

    // Select the first material and update GUI
    QModelIndex index = m_mainwin->nodesGui.mesh.matedit->materialsModel->index(0, 0);

    m_mainwin->nodesGui.mesh.matedit->materialsView->setCurrentIndex(index);
    materialSelected(index);

    // Get if the mesh has included material or materia file

    std::string matfile = m_mainwin->tbeWidget()->sceneParser()->getMaterialFile(m_target);

    m_mainwin->nodesGui.mesh.includedmat->blockSignals(true);

    if(!matfile.empty())
    {
        m_mainwin->nodesGui.mesh.openmatfile->setEnabled(true);

        m_mainwin->nodesGui.mesh.materialFilePath->setEnabled(true);
        m_mainwin->nodesGui.mesh.materialFilePath->setText(QString::fromStdString(matfile));

        m_mainwin->nodesGui.mesh.delmatfile->setEnabled(true);

        m_mainwin->nodesGui.mesh.includedmat->setChecked(false);
    }
    else if(m_mainwin->tbeWidget()->sceneParser()->isIncludedMaterialFile(m_target))
    {
        m_mainwin->nodesGui.mesh.openmatfile->setEnabled(false);

        m_mainwin->nodesGui.mesh.materialFilePath->setEnabled(false);
        m_mainwin->nodesGui.mesh.materialFilePath->clear();

        m_mainwin->nodesGui.mesh.delmatfile->setEnabled(false);

        m_mainwin->nodesGui.mesh.includedmat->setChecked(true);
    }
    else
    {
        m_mainwin->nodesGui.mesh.openmatfile->setEnabled(true);
    }

    m_mainwin->nodesGui.mesh.includedmat->blockSignals(false);

    // Update GUI by Billboard settings
    tbe::Vector2b billboard = m_target->getBillBoard();
    m_mainwin->nodesGui.mesh.billboardX->setChecked(billboard.x);
    m_mainwin->nodesGui.mesh.billboardY->setChecked(billboard.y);

    // Surroun the mesh
    m_mainwin->m_tbeWidget->selBox()->setAround(m_target, tbe::Vector4f(0, 0, 1, 0.25));

    blocker.unblock();
}
