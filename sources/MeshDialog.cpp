/* 
 * File:   MeshDialog.cpp
 * Author: b4n92uid
 * 
 * Created on 23 mai 2013, 17:09
 */

#include "MeshDialog.h"
#include "QVectorBox.h"
#include "QMeshInteractor.h"
#include "QSignalBlocker.h"
#include "MeshDialog.h"
#include "MainWindow.h"

using namespace tbe;
using namespace scene;

MeshDialog::MeshDialog(MainWindow* parent) : QDialog(parent), m_mainwin(parent)
{
    setupUi(this);

    ambiant = new QDoubleVector3Box(this, ambiant_r, ambiant_g, ambiant_b);
    diffuse = new QDoubleVector3Box(this, diffuse_r, diffuse_g, diffuse_b);
    specular = new QDoubleVector3Box(this, specular_r, specular_g, specular_b);

    rgb = new QDoubleVector3Box(this, color_r, color_g, color_b);
    clipping_framesize = new QIntVector2Box(this, clipping_x, clipping_y);
    clipping_part = new QIntVector2Box(this, clipping_part_x, clipping_part_y);

    textureModel = new QStandardItemModel(this);
    textureView->setModel(textureModel);
}

MeshDialog::~MeshDialog() { }

tbe::scene::SubMesh* MeshDialog::getSelectedSubMesh()
{
    int index = materials_select->currentIndex();

    if(index == -1)
        index = 0;

    return m_target->getSubMesh(index);
}

tbe::scene::Material* MeshDialog::getSelectedMaterial()
{
    using namespace tbe::scene;

    int index = materials_select->currentIndex();

    if(index == -1)
        index = 0;

    return m_target->getSubMesh(index)->getMaterial();
}

void MeshDialog::onMaterialSelected()
{
    if(!m_target)
        return;

    using namespace std;
    using namespace tbe;
    using namespace scene;

    SubMesh* sm = getSelectedSubMesh();
    Material* mat = getSelectedMaterial();

    // Reload textures list
    textured->setChecked(mat->isEnable(Material::TEXTURED));

    textureModel->
            removeRows(0, textureModel->rowCount());

    unsigned count = mat->getTexturesCount();
    for(unsigned i = 0; i < count; i++)
    {
        Texture tex = mat->getTexture(i);

        QString path = QString::fromStdString(tex.getFilename());

        QVariant data;
        data.setValue(tex);

        QStandardItem* item = new QStandardItem(QFileInfo(path).baseName());
        item->setData(data);

        textureModel->appendRow(item);
    }

    // Select the first texture unit and update GUI
    QModelIndex first = textureModel->index(0, 0);

    if(first.isValid())
    {
        textureView->setCurrentIndex(first);
        onTextureSelected(first);
    }

    // Update material gui flag
    foged->setChecked(mat->isEnable(Material::FOGED));
    lighted->setChecked(mat->isEnable(Material::LIGHTED));
    culltrick->setChecked(mat->isEnable(Material::VERTEX_SORT_CULL_TRICK));

    // Update material colors and lighting value
    rgb->setValue(tbe::math::vec43(mat->getColor()));
    ambiant->setValue(tbe::math::vec43(mat->getAmbient()));
    diffuse->setValue(tbe::math::vec43(mat->getDiffuse()));
    specular->setValue(tbe::math::vec43(mat->getSpecular()));
    shininess->setValue(mat->getShininess());

    // Update material blending state
    bool blend = mat->isEnable(Material::MODULATE)
            || mat->isEnable(Material::ADDITIVE)
            || mat->isEnable(Material::MULTIPLY);

    blending->setChecked(blend);

    if(blending)
    {
        if(mat->isEnable(Material::MODULATE))
            blend_modulate->setChecked(true);

        else if(mat->isEnable(Material::ADDITIVE))
            blend_additive->setChecked(true);

        else if(mat->isEnable(Material::MULTIPLY))
            blend_mul->setChecked(true);
    }

    else
        blend_modulate->setChecked(true);


    // Alpha
    bool balpha = mat->isEnable(Material::ALPHA);
    alpha->setChecked(balpha);
    alphathreshold->setValue(mat->getAlphaThershold());

    // Shader
    Shader program = mat->getShader();

    if(program.isEnable())
    {
        shader_path->setText(program.getShaderFile().c_str());
        shader_release->setEnabled(true);
    }
    else
    {
        shader_path->clear();
        shader_release->setEnabled(false);
    }

    // Attached material file
    string attachedFile = m_target->serializing().get<string>("material." + sm->getName(), "");

    if(!attachedFile.empty())
    {
        reloadMaterial->setEnabled(true);
        releaseMaterial->setEnabled(true);
        matinfo->setText(QString::fromStdString(attachedFile));
    }
    else
    {
        reloadMaterial->setEnabled(false);
        releaseMaterial->setEnabled(false);
        matinfo->clear();
    }
}

void MeshDialog::setFoged(bool stat)
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

void MeshDialog::setTextured(bool stat)
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

void MeshDialog::onTextureSelected(const QModelIndex& index)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    Texture tex = mat->getTexture(index.row());

    switch(tex.getOrigin())
    {
        case 0: texture_origin0->setChecked(true);
            break;

        case 1: texture_origin1->setChecked(true);
            break;

        case 2: texture_origin2->setChecked(true);
            break;
    }

    texture_mipmap->setChecked(tex.isGenMipMap());

    // Texture's blending
    if(mat->getTextureBlend(index.row()) == Material::MODULATE)
        texture_modulate->setChecked(true);

    else if(mat->getTextureBlend(index.row()) == Material::ADDITIVE)
        texture_additive->setChecked(true);

    else if(mat->getTextureBlend(index.row()) == Material::REPLACE)
        texture_replace->setChecked(true);

    // Clipping & animation
    cliping->setChecked(mat->isTextureClipped(index.row()));
    clipping_animation->setChecked(mat->getTextureAnimation(index.row()) > 0);
    clipping_animation_msec->setValue(mat->getTextureAnimation(index.row()));
    clipping_framesize->setValue(mat->getTextureFrameSize(index.row()));
    clipping_part->setValue(mat->getTexturePart(index.row()));
}

void MeshDialog::addTexture()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    QStringList paths = QFileDialog::getOpenFileNames(m_mainwin, "", m_mainwin->openFileName());

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

            textureModel->appendRow(item);
            mat->setTexture(tex, offset + i);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(m_mainwin, "Erreur de chargement", e.what());
        }
    }
}

void MeshDialog::delTexture()
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    QModelIndex index = textureView->currentIndex();
    mat->dropTexture(index.row());

    textureModel->removeRow(index.row());
}

void MeshDialog::textureUp()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    int srcindex = textureView->currentIndex().row();

    if(srcindex > 0)
    {
        int dstindex = srcindex - 1;

        Material* mat = getSelectedMaterial();

        Texture src = mat->getTexture(srcindex);
        Texture dst = mat->getTexture(dstindex);

        mat->setTexture(src, dstindex);
        mat->setTexture(dst, srcindex);

        QList<QStandardItem*> items = textureModel->takeRow(srcindex);
        textureModel->insertRow(dstindex, items);

        textureView
                ->setCurrentIndex(textureModel->index(dstindex, 0));
    }
}

void MeshDialog::textureDown()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    int srcindex = textureView->currentIndex().row();

    if(srcindex < textureModel->rowCount() - 1)
    {
        int dstindex = srcindex + 1;

        Material* mat = getSelectedMaterial();

        Texture src = mat->getTexture(srcindex);
        Texture dst = mat->getTexture(dstindex);

        mat->setTexture(src, dstindex);
        mat->setTexture(dst, srcindex);

        QList<QStandardItem*> items = textureModel->takeRow(srcindex);
        textureModel->insertRow(dstindex, items);

        textureView
                ->setCurrentIndex(textureModel->index(dstindex, 0));
    }
}

void MeshDialog::textureSetBlendMode()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    QModelIndex index = textureView->currentIndex();

    Material* mat = getSelectedMaterial();

    if(texture_modulate->isChecked())
        mat->setTextureBlend(Material::MODULATE, index.row());

    else if(texture_additive->isChecked())
        mat->setTextureBlend(Material::ADDITIVE, index.row());

    else if(texture_replace->isChecked())
        mat->setTextureBlend(Material::REPLACE, index.row());
}

void MeshDialog::textureSetOrigin()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    QModelIndex index = textureView->currentIndex();

    Material* mat = getSelectedMaterial();

    Texture tex = mat->getTexture(index.row());

    int origin = 0;

    if(texture_origin1->isChecked())
        origin = 1;
    else if(texture_origin2->isChecked())
        origin = 2;

    tex.load(tex.getFilename(), tex.isGenMipMap(), origin, true);

    mat->setTexture(tex, index.row());
}

void MeshDialog::textureSetMipmap()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    QModelIndex index = textureView->currentIndex();

    Material* mat = getSelectedMaterial();

    Texture tex = mat->getTexture(index.row());

    tex.load(tex.getFilename(), texture_mipmap->isChecked(), tex.getOrigin(), true);

    mat->setTexture(tex, index.row());
}

void MeshDialog::setBlend(bool stat)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(stat)
    {
        mat->enable(Material::COLORED);

        setBlendMode();
    }

    else
    {
        mat->disable(Material::ADDITIVE
                     | Material::MODULATE
                     | Material::MULTIPLY);
    }
}

void MeshDialog::setBlendMode()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    Material* mat = getSelectedMaterial();

    mat->disable(Material::MODULATE | Material::ADDITIVE | Material::MULTIPLY);

    if(blend_modulate->isChecked())
        mat->enable(Material::MODULATE);

    else if(blend_additive->isChecked())
        mat->enable(Material::ADDITIVE);

    else if(blend_mul->isChecked())
        mat->enable(Material::MULTIPLY);
}

void MeshDialog::setColor(const tbe::Vector3f& value)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    tbe::Vector4f color = tbe::math::vec34(value);
    color.w = color_a->value();

    mat->setColor(color);
}

void MeshDialog::setOpacity(double value)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setOpacity((float) value);
}

void MeshDialog::setAmbiant(tbe::Vector3f color)
{
    getSelectedMaterial()->setAmbient(tbe::math::vec34(color));
}

void MeshDialog::setDiffuse(tbe::Vector3f color)
{
    getSelectedMaterial()->setDiffuse(tbe::math::vec34(color));
}

void MeshDialog::setSpecular(tbe::Vector3f color)
{
    getSelectedMaterial()->setSpecular(tbe::math::vec34(color));
}

void MeshDialog::setShininess(double value)
{
    getSelectedMaterial()->setShininess((float) value);
}

void MeshDialog::setTextureClipping(bool state)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setTextureClipped(state);
}

void MeshDialog::setTextureFrameSize(const tbe::Vector2i& size)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setTextureFrameSize(size);
}

void MeshDialog::setTexturePart(const tbe::Vector2i& value)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setTexturePart(value);
}

void MeshDialog::setFrameAnimation(bool state)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();
    int anim = clipping_animation_msec->value();
    mat->setTextureAnimation(state ? anim : 0);
}

void MeshDialog::setFrameDuration(int value)
{
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();
    mat->setTextureAnimation(value);
}

void MeshDialog::setAlpha(bool stat)
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

void MeshDialog::setAlphaThreshold(double value)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    mat->setAlphaThershold((float) value);
}

void MeshDialog::setCullTrick(bool stat)
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

void MeshDialog::setLighted(bool stat)
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

void MeshDialog::bind(QMeshInteractor* mi)
{
    connect(attachMaterial, SIGNAL(clicked()), this, SLOT(onAttachMaterial()));
    connect(releaseMaterial, SIGNAL(clicked()), this, SLOT(onReleaseMaterial()));
    connect(reloadMaterial, SIGNAL(clicked()), this, SLOT(onReloadMaterial()));

    connect(billboardX, SIGNAL(clicked(bool)), mi, SLOT(setBillBoardX(bool)));
    connect(billboardY, SIGNAL(clicked(bool)), mi, SLOT(setBillBoardY(bool)));

    connect(castshadow, SIGNAL(clicked(bool)), mi, SLOT(setCastShadow(bool)));
    connect(receiveshadow, SIGNAL(clicked(bool)), mi, SLOT(setReceiveShadow(bool)));

    connect(computeNormal, SIGNAL(clicked()), mi, SLOT(setComputeNormal()));
    connect(computeTangent, SIGNAL(clicked()), mi, SLOT(setComputeTangent()));

    // Material

    connect(this, SIGNAL(rejected()), this, SLOT(onClose()));
    connect(this, SIGNAL(accepted()), this, SLOT(onApply()));

    connect(textured, SIGNAL(clicked(bool)), this, SLOT(setTextured(bool)));
    connect(lighted, SIGNAL(clicked(bool)), this, SLOT(setLighted(bool)));
    connect(foged, SIGNAL(clicked(bool)), this, SLOT(setFoged(bool)));
    connect(alpha, SIGNAL(clicked(bool)), this, SLOT(setAlpha(bool)));
    connect(blending, SIGNAL(clicked(bool)), this, SLOT(setBlend(bool)));
    connect(culltrick, SIGNAL(clicked(bool)), this, SLOT(setCullTrick(bool)));
    connect(rgb, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setColor(const tbe::Vector3f&)));
    connect(color_a, SIGNAL(valueChanged(double)), this, SLOT(setOpacity(double)));

    connect(ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setAmbiant(const tbe::Vector3f&)));
    connect(diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setDiffuse(const tbe::Vector3f&)));
    connect(specular, SIGNAL(valueChanged(const tbe::Vector3f&)), this, SLOT(setSpecular(const tbe::Vector3f&)));
    connect(shininess, SIGNAL(valueChanged(double)), this, SLOT(setShininess(double)));

    connect(alphathreshold, SIGNAL(valueChanged(double)), this, SLOT(setAlphaThreshold(double)));

    connect(texture_add, SIGNAL(clicked()), this, SLOT(addTexture()));
    connect(texture_del, SIGNAL(clicked()), this, SLOT(delTexture()));
    connect(texture_up, SIGNAL(clicked()), this, SLOT(textureUp()));
    connect(texture_down, SIGNAL(clicked()), this, SLOT(textureDown()));

    connect(blend_additive, SIGNAL(clicked()), this, SLOT(setBlendMode()));
    connect(blend_modulate, SIGNAL(clicked()), this, SLOT(setBlendMode()));
    connect(blend_mul, SIGNAL(clicked()), this, SLOT(setBlendMode()));

    connect(texture_additive, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));
    connect(texture_modulate, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));
    connect(texture_replace, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));
    connect(texture_origin0, SIGNAL(clicked()), this, SLOT(textureSetOrigin()));
    connect(texture_origin1, SIGNAL(clicked()), this, SLOT(textureSetOrigin()));
    connect(texture_origin2, SIGNAL(clicked()), this, SLOT(textureSetOrigin()));
    connect(texture_mipmap, SIGNAL(clicked()), this, SLOT(textureSetMipmap()));

    connect(cliping, SIGNAL(clicked(bool)), this, SLOT(setTextureClipping(bool)));
    connect(clipping_animation, SIGNAL(clicked(bool)), this, SLOT(setFrameAnimation(bool)));
    connect(clipping_framesize, SIGNAL(valueChanged(const tbe::Vector2i&)), this, SLOT(setTextureFrameSize(const tbe::Vector2i&)));
    connect(clipping_part, SIGNAL(valueChanged(const tbe::Vector2i&)), this, SLOT(setTexturePart(const tbe::Vector2i&)));
    connect(clipping_animation_msec, SIGNAL(valueChanged(int)), this, SLOT(setFrameDuration(int)));

    connect(textureView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTextureSelected(const QModelIndex &)));

    connect(shader_browse, SIGNAL(clicked()), this, SLOT(openShaderFileName()));
    connect(shader_release, SIGNAL(clicked()), this, SLOT(releaseShaderFileName()));

    connect(materials_select, SIGNAL(currentIndexChanged(int)), this, SLOT(onMaterialSelected()));
}

void MeshDialog::unbind()
{
    disconnect(attachMaterial, SIGNAL(clicked()), 0, 0);
    disconnect(releaseMaterial, SIGNAL(clicked()), 0, 0);
    disconnect(reloadMaterial, SIGNAL(clicked()), 0, 0);

    disconnect(billboardX, SIGNAL(clicked()), 0, 0);
    disconnect(billboardY, SIGNAL(clicked()), 0, 0);

    disconnect(castshadow, SIGNAL(clicked()), 0, 0);
    disconnect(receiveshadow, SIGNAL(clicked()), 0, 0);
    disconnect(computeNormal, SIGNAL(clicked()), 0, 0);
    disconnect(computeTangent, SIGNAL(clicked()), 0, 0);

    billboardX->setChecked(false);
    billboardY->setChecked(false);
    castshadow->setChecked(false);
    receiveshadow->setChecked(false);
    computeNormal->setChecked(false);
    computeTangent->setChecked(false);

    // Material 

    disconnect(this, SIGNAL(rejected()), 0, 0);
    disconnect(this, SIGNAL(accepted()), 0, 0);

    disconnect(textured, SIGNAL(clicked(bool)), 0, 0);
    disconnect(lighted, SIGNAL(clicked(bool)), 0, 0);
    disconnect(foged, SIGNAL(clicked(bool)), 0, 0);
    disconnect(alpha, SIGNAL(clicked(bool)), 0, 0);
    disconnect(blending, SIGNAL(clicked(bool)), 0, 0);
    disconnect(culltrick, SIGNAL(clicked(bool)), 0, 0);
    disconnect(color_a, SIGNAL(valueChanged(double)), 0, 0);

    disconnect(ambiant, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(diffuse, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(specular, SIGNAL(valueChanged(const tbe::Vector3f&)), 0, 0);
    disconnect(shininess, SIGNAL(valueChanged(double)), 0, 0);

    disconnect(alphathreshold, SIGNAL(valueChanged(double)), 0, 0);

    disconnect(texture_add, SIGNAL(clicked()), 0, 0);
    disconnect(texture_del, SIGNAL(clicked()), 0, 0);
    disconnect(texture_up, SIGNAL(clicked()), 0, 0);
    disconnect(texture_down, SIGNAL(clicked()), 0, 0);

    disconnect(blend_additive, SIGNAL(clicked()), 0, 0);
    disconnect(blend_modulate, SIGNAL(clicked()), 0, 0);
    disconnect(blend_mul, SIGNAL(clicked()), 0, 0);

    disconnect(texture_additive, SIGNAL(clicked()), 0, 0);
    disconnect(texture_modulate, SIGNAL(clicked()), 0, 0);
    disconnect(texture_replace, SIGNAL(clicked()), 0, 0);
    disconnect(texture_origin0, SIGNAL(clicked()), 0, 0);
    disconnect(texture_origin1, SIGNAL(clicked()), 0, 0);
    disconnect(texture_origin2, SIGNAL(clicked()), 0, 0);
    disconnect(texture_mipmap, SIGNAL(clicked()), 0, 0);

    disconnect(cliping, SIGNAL(clicked(bool)), 0, 0);
    disconnect(clipping_animation, SIGNAL(clicked(bool)), 0, 0);
    disconnect(clipping_framesize, SIGNAL(valueChanged(const tbe::Vector2i&)), 0, 0);
    disconnect(clipping_part, SIGNAL(valueChanged(const tbe::Vector2i&)), 0, 0);
    disconnect(clipping_animation_msec, SIGNAL(valueChanged(int)), 0, 0);

    disconnect(textureView, SIGNAL(clicked(const QModelIndex &)), 0, 0);

    disconnect(materials_select, SIGNAL(currentIndexChanged(int)), 0, 0);

    materials_select->clear();
}

void MeshDialog::updateMaterials(tbe::scene::Mesh* target)
{
    using namespace std;

    m_target = target;

    QSignalBlocker blocker;
    blocker
            << materials_select
            << alpha
            << alphathreshold
            << ambiant
            << blend_additive
            << blend_modulate
            << blend_mul
            << blending
            << cliping
            << clipping_animation
            << clipping_animation_msec
            << clipping_framesize
            << clipping_part
            << culltrick
            << diffuse
            << foged
            << rgb
            << specular
            << shininess;

    blocker.block();

    materials_select->clear();

    SubMesh::Array submeshs = m_target->getAllSubMesh();

    foreach(SubMesh* sm, submeshs)
    {
        Material* mat = sm->getMaterial();
        QString matName = QString("%1 (%2)").arg(sm->getName().c_str(), mat->getName().c_str());

        if(matName.isEmpty())
            matName = QString("[SubMesh sans nom #%1]").arg(materials_select->count());

        materials_select->addItem(matName);

        boost::optional<string> matFile = target->serializing()
                .get_optional<string>("material." + sm->getName());

        if(matFile)
        {
            string abspath = m_mainwin->tbeWidget()->sceneParser()->resolve(*matFile);
            m_filepath[sm->getName()] = abspath;
        }
    }

    // Select the first material and update GUI 
    materials_select->setCurrentIndex(0);
    onMaterialSelected();

    blocker.unblock();
}

void MeshDialog::update(tbe::scene::Mesh* m)
{
    QSignalBlocker blocker;
    blocker << billboardX << billboardY << castshadow << receiveshadow;

    blocker.block();

    tbe::Vector2b billboard = m->getBillBoard();
    billboardX->setChecked(billboard.x);
    billboardY->setChecked(billboard.y);

    castshadow->setChecked(m->isCastShadow());
    receiveshadow->setChecked(m->isReceiveShadow());
    computeNormal->setChecked(m->isComputeNormals());
    computeTangent->setChecked(m->isComputeTangent());

    updateMaterials(m);

    blocker.unblock();
}

void MeshDialog::openShaderFileName()
{
    QString filename = QFileDialog::getOpenFileName(m_mainwin, "", m_mainwin->openFileName());

    if(!filename.isEmpty())
    {
        try
        {
            Material* mat = getSelectedMaterial();

            Shader program;
            program.parseShaderFile(filename.toStdString());

            mat->setShader(program);

            shader_path->setText(filename);
            shader_release->setEnabled(true);
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(m_mainwin, "Chargement du Shader", e.what());
        }
    }
}

void MeshDialog::releaseShaderFileName()
{
    Material* mat = getSelectedMaterial();
    mat->setShader(Shader());

    shader_path->clear();
    shader_release->setEnabled(false);
}

void MeshDialog::onClose() { }

void MeshDialog::onApply()
{
    using namespace std;
    using namespace boost;

    Material* mat = getSelectedMaterial();
    SubMesh* smesh = getSelectedSubMesh();

    string smeshname = smesh->getName();
    string matpath;

    if(m_filepath.count(smeshname))
        matpath = m_filepath[smeshname];
    else
        matpath = QFileDialog::getSaveFileName(this, "", m_mainwin->openFileName()).toStdString();

    if(!matpath.empty())
    {
        MaterialManager* mm = MaterialManager::get();
        rtree materialTree = mm->serialize(mat->getName(), matpath);

        property_tree::write_info(matpath, materialTree);

        smesh->getOwner()->serializing().put("material." + smeshname, matpath);

        m_mainwin->statusBar()->showMessage(QString("Matériau enregistré (%1)").arg(matpath.c_str()), 2000);

        m_mainwin->notifyChange();
    }
}

void MeshDialog::onAttachMaterial()
{
    QString filename = QFileDialog::getOpenFileName(m_mainwin, "", m_mainwin->openFileName(),
                                                    "Material (*.material);;Tout les fichiers (*.*)");
    if(!filename.isEmpty())
    {
        try
        {
            std::string path = filename.toStdString();

            SubMesh* smesh = getSelectedSubMesh();

            MaterialManager* matmng = MaterialManager::get();
            Material* newmat = matmng->loadMaterial(path);

            smesh->setMaterial(newmat);
            smesh->getOwner()->serializing().put("material." + smesh->getName(), path);

            reloadMaterial->setEnabled(true);
            releaseMaterial->setEnabled(true);

            matinfo->setText(filename);

            m_mainwin->notifyChange();
        }
        catch(std::exception& e)
        {
            QMessageBox::critical(this, "Erreur de chargement",
                                  QString("Le fichier na pas pu etre chargé correctement\n%1").arg(+e.what()));
        }
    }
}

void MeshDialog::onReleaseMaterial()
{
    SubMesh* smesh = getSelectedSubMesh();
    std::string matname = smesh->getMaterial()->getName();

    Material* prevMat = MaterialManager::get()->backupMaterial(matname);

    if(prevMat)
    {
        smesh->setMaterial(prevMat);
        reloadMaterial->setEnabled(false);
        releaseMaterial->setEnabled(false);
        matinfo->clear();
    }
    else
        QMessageBox::warning(this, "Erreur", "Aucun matèrieau précedent trouvé !");
}

void MeshDialog::onReloadMaterial()
{
    SubMesh* smesh = getSelectedSubMesh();
    std::string smeshname = smesh->getName();
    std::string matpath = m_filepath[smeshname];

    try
    {
        Material* material = MaterialManager::get()->loadMaterial(matpath, true);
        smesh->setMaterial(material);
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(this, "Erreur de chargement",
                              QString("Le fichier na pas pu etre chargé correctement\n%1").arg(+e.what()));
    }
}
