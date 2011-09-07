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
    using namespace tbe::scene;

    QVariant userdata;
    userdata.setValue((QNodeInteractor*)this);

    QStandardItem* itemType = new QStandardItem("Mesh");
    itemType->setIcon(QIcon(":/Medias/medias/mesh.png"));
    itemType->setData(userdata);

    QStandardItem* itemName = new QStandardItem(QString::fromStdString(m_target->getName()));
    itemName->setData(userdata);

    QItemsList items;
    items << itemType << itemName;

    QNodeInteractor* parent = m_mainwin->m_tbeWidget->fetchInterface(m_target->getParent());

    if(m_mainwin->nodesGui.nodeItemBinder.count(parent))
        m_mainwin->nodesGui.nodeItemBinder[parent]->appendRow(items);
    else
        m_mainwin->nodesGui.nodesListModel->appendRow(items);

    m_mainwin->nodesGui.nodeItemBinder[this] = itemType;

    m_mainwin->notifyChanges(true);
}

QMeshInteractor::~QMeshInteractor()
{
    m_mainwin->m_tbeWidget->meshDelete(m_target);
}

tbe::scene::Material* QMeshInteractor::getSelectedMaterial()
{
    using namespace tbe::scene;

    QModelIndex index = m_mainwin->nodesGui.meshTab.materialsView->currentIndex();

    if(index.isValid())
        return m_mainwin->nodesGui.meshTab.materialsModel
            ->itemFromIndex(index)->data().value<Material*>();
    else
        return NULL;
}

void QMeshInteractor::setSaveMaterial(bool stat)
{
    if(!m_target)
        return;

    m_target->setOutputMaterial(stat);

    m_mainwin->nodesGui.meshTab.openmatedit->setEnabled(stat);
    m_mainwin->nodesGui.meshTab.opacity->setEnabled(stat);

    m_mainwin->notifyChanges(true);
}

void QMeshInteractor::materialSelected(const QModelIndex& index)
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    // Textures

    m_mainwin->nodesGui.meshTab.matedit->textured->setChecked(mat->isEnable(Material::TEXTURED));

    m_mainwin->nodesGui.meshTab.textureModel->
            removeRows(0, m_mainwin->nodesGui.meshTab.textureModel->rowCount());

    unsigned count = mat->getTexturesCount();
    for(unsigned i = 0; i < count; i++)
    {
        Texture tex = mat->getTexture(i);

        QString path = QString::fromStdString(tex.getFilename());

        QVariant data;
        data.setValue(tex);

        QStandardItem* item = new QStandardItem(QFileInfo(path).baseName());
        item->setData(data);

        m_mainwin->nodesGui.meshTab.textureModel->appendRow(item);
    }

    QModelIndex first = m_mainwin->nodesGui.meshTab.textureModel->index(0, 0);

    m_mainwin->nodesGui.meshTab.matedit->textureView->setCurrentIndex(first);
    textureSelected(first);

    // Material stat
    m_mainwin->nodesGui.meshTab.matedit->foged->setChecked(mat->isEnable(Material::FOGED));
    m_mainwin->nodesGui.meshTab.matedit->lighted->setChecked(mat->isEnable(Material::LIGHTED));
    m_mainwin->nodesGui.meshTab.matedit->culltrick->setChecked(mat->isEnable(Material::VERTEX_SORT_CULL_TRICK));

    // Blending stat
    bool blending = mat->isEnable(Material::BLEND_MOD)
            || mat->isEnable(Material::BLEND_ADD)
            || mat->isEnable(Material::BLEND_MUL);

    m_mainwin->nodesGui.meshTab.matedit->blending->setChecked(blending);

    if(blending)
    {
        if(mat->isEnable(Material::BLEND_MOD))
            m_mainwin->nodesGui.meshTab.matedit->blend_modulate->setChecked(true);

        else if(mat->isEnable(Material::BLEND_ADD))
            m_mainwin->nodesGui.meshTab.matedit->blend_additive->setChecked(true);

        else if(mat->isEnable(Material::BLEND_MUL))
            m_mainwin->nodesGui.meshTab.matedit->blend_mul->setChecked(true);
    }

    else
        m_mainwin->nodesGui.meshTab.matedit->blend_modulate->setChecked(true);

    m_mainwin->nodesGui.meshTab.opacity->setValue(m_target->getOpacity());

    // Alpha
    bool alpha = mat->isEnable(Material::ALPHA);

    m_mainwin->nodesGui.meshTab.matedit->alpha->setChecked(alpha);
    m_mainwin->nodesGui.meshTab.matedit->alphathreshold->setValue(mat->getAlphaThershold());
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

    m_mainwin->notifyChanges(true);
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

    m_mainwin->notifyChanges(true);
}

void QMeshInteractor::textureSelected(const QModelIndex& index)
{
    if(!m_target)
        return;

    using namespace tbe::scene;

    Material* mat = getSelectedMaterial();

    if(mat->getTextureBlend(index.row()) == Material::MODULATE)
        m_mainwin->nodesGui.meshTab.matedit->texture_modulate->setChecked(true);

    else if(mat->getTextureBlend(index.row()) == Material::ADDITIVE)
        m_mainwin->nodesGui.meshTab.matedit->texture_additive->setChecked(true);

    else if(mat->getTextureBlend(index.row()) == Material::REPLACE)
        m_mainwin->nodesGui.meshTab.matedit->texture_replace->setChecked(true);
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

            m_mainwin->nodesGui.meshTab.textureModel->appendRow(item);
            mat->setTexture(tex, offset + i);

            m_mainwin->notifyChanges(true);
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

    QModelIndex index = m_mainwin->nodesGui.meshTab.matedit->textureView->currentIndex();
    mat->dropTexture(index.row());

    m_mainwin->nodesGui.meshTab.textureModel->removeRow(index.row());

    m_mainwin->notifyChanges(true);
}

void QMeshInteractor::textureUp()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    int srcindex = m_mainwin->nodesGui.meshTab.matedit->textureView->currentIndex().row();

    if(srcindex > 0)
    {
        int dstindex = srcindex - 1;

        Material* mat = getSelectedMaterial();

        Texture src = mat->getTexture(srcindex);
        Texture dst = mat->getTexture(dstindex);

        mat->setTexture(src, dstindex);
        mat->setTexture(dst, srcindex);

        QList<QStandardItem*> items = m_mainwin->nodesGui.meshTab.textureModel->takeRow(srcindex);
        m_mainwin->nodesGui.meshTab.textureModel->insertRow(dstindex, items);

        m_mainwin->nodesGui.meshTab.matedit->textureView
                ->setCurrentIndex(m_mainwin->nodesGui.meshTab.textureModel->index(dstindex, 0));

        m_mainwin->notifyChanges(true);
    }
}

void QMeshInteractor::textureDown()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    int srcindex = m_mainwin->nodesGui.meshTab.matedit->textureView->currentIndex().row();

    if(srcindex < m_mainwin->nodesGui.meshTab.textureModel->rowCount() - 1)
    {
        int dstindex = srcindex + 1;

        Material* mat = getSelectedMaterial();

        Texture src = mat->getTexture(srcindex);
        Texture dst = mat->getTexture(dstindex);

        mat->setTexture(src, dstindex);
        mat->setTexture(dst, srcindex);

        QList<QStandardItem*> items = m_mainwin->nodesGui.meshTab.textureModel->takeRow(srcindex);
        m_mainwin->nodesGui.meshTab.textureModel->insertRow(dstindex, items);

        m_mainwin->nodesGui.meshTab.matedit->textureView
                ->setCurrentIndex(m_mainwin->nodesGui.meshTab.textureModel->index(dstindex, 0));

        m_mainwin->notifyChanges(true);
    }
}

void QMeshInteractor::textureSetBlendMode()
{
    if(!m_target)
        return;

    using namespace tbe;
    using namespace scene;

    QModelIndex index = m_mainwin->nodesGui.meshTab.matedit->textureView->currentIndex();

    Material* mat = getSelectedMaterial();

    if(m_mainwin->nodesGui.meshTab.matedit->texture_modulate->isChecked())
        mat->setTextureBlend(Material::MODULATE, index.row());

    else if(m_mainwin->nodesGui.meshTab.matedit->texture_additive->isChecked())
        mat->setTextureBlend(Material::ADDITIVE, index.row());

    else if(m_mainwin->nodesGui.meshTab.matedit->texture_replace->isChecked())
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

    if(m_mainwin->nodesGui.meshTab.matedit->blend_modulate->isChecked())
        mat->enable(Material::BLEND_MOD);

    else if(m_mainwin->nodesGui.meshTab.matedit->blend_additive->isChecked())
        mat->enable(Material::BLEND_ADD);

    else if(m_mainwin->nodesGui.meshTab.matedit->blend_mul->isChecked())
        mat->enable(Material::BLEND_MUL);
}

void QMeshInteractor::setBillBoard()
{
    tbe::Vector2b apply;
    apply.x = m_mainwin->nodesGui.meshTab.billboardX->isChecked();
    apply.y = m_mainwin->nodesGui.meshTab.billboardY->isChecked();

    m_target->setBillBoard(apply);
}

void QMeshInteractor::setOpacity(double value)
{
    if(!m_target)
        return;

    m_target->setOpacity((float)value);
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

void QMeshInteractor::select()
{
    deselect();

    QNodeInteractor::select();

    connect(m_mainwin->nodesGui.meshTab.saveMaterials, SIGNAL(clicked(bool)), this, SLOT(setSaveMaterial(bool)));
    connect(m_mainwin->nodesGui.meshTab.openmatedit, SIGNAL(clicked()), m_mainwin->nodesGui.meshTab.matedit, SLOT(show()));

    connect(m_mainwin->nodesGui.meshTab.matedit->textured, SIGNAL(clicked(bool)), this, SLOT(setTextured(bool)));
    connect(m_mainwin->nodesGui.meshTab.matedit->lighted, SIGNAL(clicked(bool)), this, SLOT(setLighted(bool)));
    connect(m_mainwin->nodesGui.meshTab.matedit->foged, SIGNAL(clicked(bool)), this, SLOT(setFoged(bool)));
    connect(m_mainwin->nodesGui.meshTab.matedit->alpha, SIGNAL(clicked(bool)), this, SLOT(setAlpha(bool)));
    connect(m_mainwin->nodesGui.meshTab.matedit->blending, SIGNAL(clicked(bool)), this, SLOT(setBlend(bool)));
    connect(m_mainwin->nodesGui.meshTab.matedit->culltrick, SIGNAL(clicked(bool)), this, SLOT(setCullTrick(bool)));

    connect(m_mainwin->nodesGui.meshTab.opacity, SIGNAL(valueChanged(double)), this, SLOT(setOpacity(double)));
    connect(m_mainwin->nodesGui.meshTab.billboardX, SIGNAL(clicked()), this, SLOT(setBillBoard()));
    connect(m_mainwin->nodesGui.meshTab.billboardY, SIGNAL(clicked()), this, SLOT(setBillBoard()));

    connect(m_mainwin->nodesGui.meshTab.matedit->alphathreshold, SIGNAL(valueChanged(double)), this, SLOT(setAlphaThreshold(double)));

    connect(m_mainwin->nodesGui.meshTab.matedit->add, SIGNAL(clicked()), this, SLOT(addTexture()));
    connect(m_mainwin->nodesGui.meshTab.matedit->del, SIGNAL(clicked()), this, SLOT(delTexture()));
    connect(m_mainwin->nodesGui.meshTab.matedit->up, SIGNAL(clicked()), this, SLOT(textureUp()));
    connect(m_mainwin->nodesGui.meshTab.matedit->down, SIGNAL(clicked()), this, SLOT(textureDown()));

    connect(m_mainwin->nodesGui.meshTab.matedit->blend_additive, SIGNAL(clicked()), this, SLOT(materialSetBlendMode()));
    connect(m_mainwin->nodesGui.meshTab.matedit->blend_modulate, SIGNAL(clicked()), this, SLOT(materialSetBlendMode()));
    connect(m_mainwin->nodesGui.meshTab.matedit->blend_mul, SIGNAL(clicked()), this, SLOT(materialSetBlendMode()));

    connect(m_mainwin->nodesGui.meshTab.matedit->texture_additive, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));
    connect(m_mainwin->nodesGui.meshTab.matedit->texture_modulate, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));
    connect(m_mainwin->nodesGui.meshTab.matedit->texture_replace, SIGNAL(clicked()), this, SLOT(textureSetBlendMode()));

    connect(m_mainwin->nodesGui.meshTab.matedit->textureView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(textureSelected(const QModelIndex &)));

    connect(m_mainwin->nodesGui.meshTab.materialsView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(materialSelected(const QModelIndex &)));

    update();


    QStandardItem* item = m_mainwin->nodesGui.nodeItemBinder[this];

    m_mainwin->nodesGui.nodesListView->blockSignals(true);
    m_mainwin->nodesGui.nodesListView->setCurrentIndex(m_mainwin->nodesGui.nodesListModel->indexFromItem(item));
    m_mainwin->nodesGui.nodesListView->blockSignals(false);

    m_mainwin->nodesGui.attribTab->setCurrentIndex(0);
}

void QMeshInteractor::deselect()
{
    QNodeInteractor::deselect();

    disconnect(m_mainwin->nodesGui.meshTab.saveMaterials, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.openmatedit, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.meshTab.matedit->textured, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->lighted, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->foged, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->alpha, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->blending, SIGNAL(clicked(bool)), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->culltrick, SIGNAL(clicked(bool)), 0, 0);

    disconnect(m_mainwin->nodesGui.meshTab.opacity, SIGNAL(valueChanged(double)), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.billboardX, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.billboardY, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.meshTab.matedit->alphathreshold, SIGNAL(valueChanged(double)), 0, 0);

    disconnect(m_mainwin->nodesGui.meshTab.matedit->add, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->del, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->up, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->down, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.meshTab.matedit->blend_additive, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->blend_modulate, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->blend_mul, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.meshTab.matedit->texture_additive, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->texture_modulate, SIGNAL(clicked()), 0, 0);
    disconnect(m_mainwin->nodesGui.meshTab.matedit->texture_replace, SIGNAL(clicked()), 0, 0);

    disconnect(m_mainwin->nodesGui.meshTab.matedit->textureView, SIGNAL(clicked(const QModelIndex &)), 0, 0);

    disconnect(m_mainwin->nodesGui.meshTab.materialsView, SIGNAL(clicked(const QModelIndex &)), 0, 0);
}

void QMeshInteractor::update()
{
    using namespace tbe::scene;

    QNodeInteractor::update();

    m_mainwin->nodesGui.meshTab.materialsModel->
            removeRows(0, m_mainwin->nodesGui.meshTab.materialsModel->rowCount());

    Material::Array matarr = m_target->getAllMaterial();

    foreach(Material* mat, matarr)
    {
        QVariant data;
        data.setValue<Material*>(mat);

        QStandardItem* item = new QStandardItem(QString::fromStdString(mat->getName()));
        item->setData(data);

        m_mainwin->nodesGui.meshTab.materialsModel->appendRow(item);
    }

    m_mainwin->nodesGui.meshTab.opacity->setValue(m_target->getOpacity());

    QModelIndex index = m_mainwin->nodesGui.meshTab.materialsModel->index(0, 0);

    m_mainwin->nodesGui.meshTab.materialsView->setCurrentIndex(index);
    materialSelected(index);

    bool matset = m_target->isOutputMaterial();

    m_mainwin->nodesGui.meshTab.saveMaterials->setEnabled(true);

    m_mainwin->nodesGui.meshTab.saveMaterials->setChecked(matset);
    m_mainwin->nodesGui.meshTab.openmatedit->setEnabled(matset);
    m_mainwin->nodesGui.meshTab.opacity->setEnabled(matset);

    tbe::Vector2b billboard = m_target->getBillBoard();
    m_mainwin->nodesGui.meshTab.billboardX->setChecked(billboard.x);
    m_mainwin->nodesGui.meshTab.billboardY->setChecked(billboard.y);



    using namespace tbe;
    using namespace scene;

    Grid* grid = m_mainwin->m_tbeWidget->getGrid();
    Box* axe = m_mainwin->m_tbeWidget->getAxe();

    AABB selAabb = m_target->getAabb();

    axe->setEnable(true);
    axe->setMatrix(m_target->getAbsoluteMatrix());
    axe->setPos(m_target->getAbsoluteMatrix() * selAabb.getCenter());
    axe->setSize(selAabb.getSize() / 2.0f + 0.01f);
    axe->setColor(Vector4f(0, 0, 1, 0.25));

    grid->setPos(Vector3f::Y(axe->getPos().y));
}

QMeshInteractor* QMeshInteractor::clone()
{
    using namespace tbe::scene;

    QMesh* mesh = m_mainwin->m_tbeWidget->meshClone(m_target);

    return mesh;
}
