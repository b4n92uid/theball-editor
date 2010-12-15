/********************************************************************************
** Form generated from reading UI file 'interfacexr2792.ui'
**
** Created: Tue 14. Dec 17:40:04 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef INTERFACEXR2792_H
#define INTERFACEXR2792_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "QTBEngine.h"

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QAction *actionOuvrire;
    QAction *actionEnregistrer;
    QAction *actionEnregistrer_sous;
    QAction *actionQuitter;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_8;
    QTBEngine *glwidget;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_9;
    QLabel *infoText;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QFormLayout *formLayout_3;
    QLabel *label_6;
    QLineEdit *node_name;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QDoubleSpinBox *node_pos_x;
    QDoubleSpinBox *node_pos_y;
    QDoubleSpinBox *node_pos_z;
    QTabWidget *tabWidget_2;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_10;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *node_mesh_add;
    QPushButton *node_mesh_clone;
    QPushButton *node_mesh_del;
    QWidget *tab_5;
    QFormLayout *formLayout;
    QLabel *label_19;
    QDoubleSpinBox *node_water_deform;
    QLabel *label_18;
    QHBoxLayout *horizontalLayout_9;
    QDoubleSpinBox *node_water_size_x;
    QDoubleSpinBox *node_water_size_y;
    QLabel *label_17;
    QHBoxLayout *horizontalLayout_8;
    QDoubleSpinBox *node_water_uvrepeat_x;
    QDoubleSpinBox *node_water_uvrepeat_y;
    QLabel *label_16;
    QDoubleSpinBox *node_water_speed;
    QLabel *label_15;
    QDoubleSpinBox *node_water_blend;
    QHBoxLayout *horizontalLayout_24;
    QPushButton *node_water_add;
    QPushButton *node_water_clone;
    QPushButton *node_water_del;
    QSpacerItem *verticalSpacer_4;
    QWidget *tab_6;
    QFormLayout *formLayout_2;
    QLabel *label_20;
    QHBoxLayout *horizontalLayout_10;
    QDoubleSpinBox *node_particles_gravity_x;
    QDoubleSpinBox *node_particles_gravity_y;
    QDoubleSpinBox *node_particles_gravity_z;
    QLabel *label_21;
    QDoubleSpinBox *node_particles_freemove;
    QLabel *label_23;
    QDoubleSpinBox *node_particles_lifeinit;
    QLabel *label_24;
    QDoubleSpinBox *node_particles_lifedown;
    QLabel *label_25;
    QLabel *label_26;
    QHBoxLayout *horizontalLayout_23;
    QLineEdit *node_particles_texture;
    QPushButton *node_particles_texture_browse;
    QCheckBox *node_particles_continousmide;
    QHBoxLayout *horizontalLayout;
    QSpinBox *node_particles_number;
    QPushButton *node_particles_build;
    QHBoxLayout *horizontalLayout_25;
    QPushButton *node_particles_add;
    QPushButton *node_particles_clone;
    QPushButton *node_particles_del;
    QTreeView *node_list;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_4;
    QFormLayout *formLayout_4;
    QLabel *label_7;
    QComboBox *light_type;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QDoubleSpinBox *light_pos_x;
    QDoubleSpinBox *light_pos_y;
    QDoubleSpinBox *light_pos_z;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_19;
    QDoubleSpinBox *light_ambiant_x;
    QDoubleSpinBox *light_ambiant_y;
    QDoubleSpinBox *light_ambiant_z;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_20;
    QDoubleSpinBox *light_diffuse_x;
    QDoubleSpinBox *light_diffuse_y;
    QDoubleSpinBox *light_diffuse_z_2;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_21;
    QDoubleSpinBox *light_specular_x;
    QDoubleSpinBox *light_specular_y;
    QDoubleSpinBox *light_specular_z;
    QLabel *label_11;
    QDoubleSpinBox *light_shininess;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_22;
    QDoubleSpinBox *light_scene_x;
    QDoubleSpinBox *light_scene_y;
    QDoubleSpinBox *light_scene_z;
    QTreeView *node_type;
    QHBoxLayout *horizontalLayout_26;
    QPushButton *light_add;
    QPushButton *light_clone;
    QPushButton *light_del;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *skybox_enable;
    QFormLayout *formLayout_5;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout_16;
    QLineEdit *skybox_front;
    QPushButton *skybox_front_browse;
    QLabel *label_14;
    QHBoxLayout *horizontalLayout_15;
    QLineEdit *skybox_back;
    QPushButton *skybox_back_browse;
    QLabel *label_22;
    QHBoxLayout *horizontalLayout_14;
    QLineEdit *skybox_top;
    QPushButton *skybox_top_browse;
    QLabel *label_27;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *skybox_bottom;
    QPushButton *skybox_bottom_browse;
    QLabel *label_28;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *skybox_left;
    QPushButton *skybox_left_browse;
    QLabel *label_29;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *skybox_right;
    QPushButton *skybox_right_browse;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *skybox_apply;
    QGroupBox *fog_enable;
    QFormLayout *formLayout_6;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_17;
    QDoubleSpinBox *fog_x;
    QDoubleSpinBox *fog_y;
    QDoubleSpinBox *fog_z;
    QLabel *label_5;
    QDoubleSpinBox *fog_start;
    QLabel *label_4;
    QDoubleSpinBox *fog_end;
    QSpacerItem *verticalSpacer_5;
    QMenuBar *menubar;
    QMenu *menuFichier;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->resize(779, 730);
        actionOuvrire = new QAction(mainWindow);
        actionOuvrire->setObjectName(QString::fromUtf8("actionOuvrire"));
        actionEnregistrer = new QAction(mainWindow);
        actionEnregistrer->setObjectName(QString::fromUtf8("actionEnregistrer"));
        actionEnregistrer_sous = new QAction(mainWindow);
        actionEnregistrer_sous->setObjectName(QString::fromUtf8("actionEnregistrer_sous"));
        actionQuitter = new QAction(mainWindow);
        actionQuitter->setObjectName(QString::fromUtf8("actionQuitter"));
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_7 = new QHBoxLayout(centralwidget);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        glwidget = new QTBEngine(centralwidget);
        glwidget->setObjectName(QString::fromUtf8("glwidget"));

        verticalLayout_8->addWidget(glwidget);

        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_9 = new QVBoxLayout(groupBox_5);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        infoText = new QLabel(groupBox_5);
        infoText->setObjectName(QString::fromUtf8("infoText"));

        verticalLayout_9->addWidget(infoText);


        verticalLayout_8->addWidget(groupBox_5);

        verticalLayout_8->setStretch(0, 9);

        horizontalLayout_7->addLayout(verticalLayout_8);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        formLayout_3 = new QFormLayout(groupBox);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_6);

        node_name = new QLineEdit(groupBox);
        node_name->setObjectName(QString::fromUtf8("node_name"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, node_name);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        node_pos_x = new QDoubleSpinBox(groupBox);
        node_pos_x->setObjectName(QString::fromUtf8("node_pos_x"));
        node_pos_x->setMinimum(-99);

        horizontalLayout_2->addWidget(node_pos_x);

        node_pos_y = new QDoubleSpinBox(groupBox);
        node_pos_y->setObjectName(QString::fromUtf8("node_pos_y"));
        node_pos_y->setMinimum(-99);

        horizontalLayout_2->addWidget(node_pos_y);

        node_pos_z = new QDoubleSpinBox(groupBox);
        node_pos_z->setObjectName(QString::fromUtf8("node_pos_z"));
        node_pos_z->setMinimum(-99);

        horizontalLayout_2->addWidget(node_pos_z);


        formLayout_3->setLayout(1, QFormLayout::FieldRole, horizontalLayout_2);


        verticalLayout_3->addWidget(groupBox);

        tabWidget_2 = new QTabWidget(tab);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_10 = new QVBoxLayout(tab_4);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_3);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        node_mesh_add = new QPushButton(tab_4);
        node_mesh_add->setObjectName(QString::fromUtf8("node_mesh_add"));

        horizontalLayout_18->addWidget(node_mesh_add);

        node_mesh_clone = new QPushButton(tab_4);
        node_mesh_clone->setObjectName(QString::fromUtf8("node_mesh_clone"));

        horizontalLayout_18->addWidget(node_mesh_clone);

        node_mesh_del = new QPushButton(tab_4);
        node_mesh_del->setObjectName(QString::fromUtf8("node_mesh_del"));

        horizontalLayout_18->addWidget(node_mesh_del);


        verticalLayout_10->addLayout(horizontalLayout_18);

        tabWidget_2->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        formLayout = new QFormLayout(tab_5);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_19 = new QLabel(tab_5);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_19);

        node_water_deform = new QDoubleSpinBox(tab_5);
        node_water_deform->setObjectName(QString::fromUtf8("node_water_deform"));
        node_water_deform->setSingleStep(0.1);

        formLayout->setWidget(2, QFormLayout::FieldRole, node_water_deform);

        label_18 = new QLabel(tab_5);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_18);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        node_water_size_x = new QDoubleSpinBox(tab_5);
        node_water_size_x->setObjectName(QString::fromUtf8("node_water_size_x"));

        horizontalLayout_9->addWidget(node_water_size_x);

        node_water_size_y = new QDoubleSpinBox(tab_5);
        node_water_size_y->setObjectName(QString::fromUtf8("node_water_size_y"));

        horizontalLayout_9->addWidget(node_water_size_y);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout_9);

        label_17 = new QLabel(tab_5);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_17);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        node_water_uvrepeat_x = new QDoubleSpinBox(tab_5);
        node_water_uvrepeat_x->setObjectName(QString::fromUtf8("node_water_uvrepeat_x"));

        horizontalLayout_8->addWidget(node_water_uvrepeat_x);

        node_water_uvrepeat_y = new QDoubleSpinBox(tab_5);
        node_water_uvrepeat_y->setObjectName(QString::fromUtf8("node_water_uvrepeat_y"));

        horizontalLayout_8->addWidget(node_water_uvrepeat_y);


        formLayout->setLayout(4, QFormLayout::FieldRole, horizontalLayout_8);

        label_16 = new QLabel(tab_5);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_16);

        node_water_speed = new QDoubleSpinBox(tab_5);
        node_water_speed->setObjectName(QString::fromUtf8("node_water_speed"));
        node_water_speed->setSingleStep(0.1);

        formLayout->setWidget(5, QFormLayout::FieldRole, node_water_speed);

        label_15 = new QLabel(tab_5);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_15);

        node_water_blend = new QDoubleSpinBox(tab_5);
        node_water_blend->setObjectName(QString::fromUtf8("node_water_blend"));
        node_water_blend->setSingleStep(0.1);

        formLayout->setWidget(6, QFormLayout::FieldRole, node_water_blend);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        node_water_add = new QPushButton(tab_5);
        node_water_add->setObjectName(QString::fromUtf8("node_water_add"));

        horizontalLayout_24->addWidget(node_water_add);

        node_water_clone = new QPushButton(tab_5);
        node_water_clone->setObjectName(QString::fromUtf8("node_water_clone"));

        horizontalLayout_24->addWidget(node_water_clone);

        node_water_del = new QPushButton(tab_5);
        node_water_del->setObjectName(QString::fromUtf8("node_water_del"));

        horizontalLayout_24->addWidget(node_water_del);


        formLayout->setLayout(8, QFormLayout::SpanningRole, horizontalLayout_24);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(7, QFormLayout::SpanningRole, verticalSpacer_4);

        tabWidget_2->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        formLayout_2 = new QFormLayout(tab_6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_20 = new QLabel(tab_6);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_20);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        node_particles_gravity_x = new QDoubleSpinBox(tab_6);
        node_particles_gravity_x->setObjectName(QString::fromUtf8("node_particles_gravity_x"));
        node_particles_gravity_x->setMinimum(-99);

        horizontalLayout_10->addWidget(node_particles_gravity_x);

        node_particles_gravity_y = new QDoubleSpinBox(tab_6);
        node_particles_gravity_y->setObjectName(QString::fromUtf8("node_particles_gravity_y"));
        node_particles_gravity_y->setMinimum(-99);

        horizontalLayout_10->addWidget(node_particles_gravity_y);

        node_particles_gravity_z = new QDoubleSpinBox(tab_6);
        node_particles_gravity_z->setObjectName(QString::fromUtf8("node_particles_gravity_z"));
        node_particles_gravity_z->setMinimum(-99);

        horizontalLayout_10->addWidget(node_particles_gravity_z);


        formLayout_2->setLayout(1, QFormLayout::FieldRole, horizontalLayout_10);

        label_21 = new QLabel(tab_6);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_21);

        node_particles_freemove = new QDoubleSpinBox(tab_6);
        node_particles_freemove->setObjectName(QString::fromUtf8("node_particles_freemove"));
        node_particles_freemove->setSingleStep(0.1);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, node_particles_freemove);

        label_23 = new QLabel(tab_6);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_23);

        node_particles_lifeinit = new QDoubleSpinBox(tab_6);
        node_particles_lifeinit->setObjectName(QString::fromUtf8("node_particles_lifeinit"));
        node_particles_lifeinit->setSingleStep(0.1);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, node_particles_lifeinit);

        label_24 = new QLabel(tab_6);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_24);

        node_particles_lifedown = new QDoubleSpinBox(tab_6);
        node_particles_lifedown->setObjectName(QString::fromUtf8("node_particles_lifedown"));
        node_particles_lifedown->setSingleStep(0.1);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, node_particles_lifedown);

        label_25 = new QLabel(tab_6);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_25);

        label_26 = new QLabel(tab_6);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_26);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        node_particles_texture = new QLineEdit(tab_6);
        node_particles_texture->setObjectName(QString::fromUtf8("node_particles_texture"));

        horizontalLayout_23->addWidget(node_particles_texture);

        node_particles_texture_browse = new QPushButton(tab_6);
        node_particles_texture_browse->setObjectName(QString::fromUtf8("node_particles_texture_browse"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(node_particles_texture_browse->sizePolicy().hasHeightForWidth());
        node_particles_texture_browse->setSizePolicy(sizePolicy1);

        horizontalLayout_23->addWidget(node_particles_texture_browse);

        horizontalLayout_23->setStretch(0, 5);
        horizontalLayout_23->setStretch(1, 1);

        formLayout_2->setLayout(6, QFormLayout::FieldRole, horizontalLayout_23);

        node_particles_continousmide = new QCheckBox(tab_6);
        node_particles_continousmide->setObjectName(QString::fromUtf8("node_particles_continousmide"));

        formLayout_2->setWidget(7, QFormLayout::FieldRole, node_particles_continousmide);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        node_particles_number = new QSpinBox(tab_6);
        node_particles_number->setObjectName(QString::fromUtf8("node_particles_number"));

        horizontalLayout->addWidget(node_particles_number);

        node_particles_build = new QPushButton(tab_6);
        node_particles_build->setObjectName(QString::fromUtf8("node_particles_build"));
        sizePolicy1.setHeightForWidth(node_particles_build->sizePolicy().hasHeightForWidth());
        node_particles_build->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(node_particles_build);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 1);

        formLayout_2->setLayout(5, QFormLayout::FieldRole, horizontalLayout);

        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        node_particles_add = new QPushButton(tab_6);
        node_particles_add->setObjectName(QString::fromUtf8("node_particles_add"));

        horizontalLayout_25->addWidget(node_particles_add);

        node_particles_clone = new QPushButton(tab_6);
        node_particles_clone->setObjectName(QString::fromUtf8("node_particles_clone"));

        horizontalLayout_25->addWidget(node_particles_clone);

        node_particles_del = new QPushButton(tab_6);
        node_particles_del->setObjectName(QString::fromUtf8("node_particles_del"));

        horizontalLayout_25->addWidget(node_particles_del);


        formLayout_2->setLayout(8, QFormLayout::SpanningRole, horizontalLayout_25);

        tabWidget_2->addTab(tab_6, QString());

        verticalLayout_3->addWidget(tabWidget_2);

        node_list = new QTreeView(tab);
        node_list->setObjectName(QString::fromUtf8("node_list"));

        verticalLayout_3->addWidget(node_list);

        verticalLayout_3->setStretch(2, 1);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        formLayout_4 = new QFormLayout(groupBox_4);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_7);

        light_type = new QComboBox(groupBox_4);
        light_type->setObjectName(QString::fromUtf8("light_type"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, light_type);

        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        light_pos_x = new QDoubleSpinBox(groupBox_4);
        light_pos_x->setObjectName(QString::fromUtf8("light_pos_x"));
        light_pos_x->setMinimum(-99);

        horizontalLayout_3->addWidget(light_pos_x);

        light_pos_y = new QDoubleSpinBox(groupBox_4);
        light_pos_y->setObjectName(QString::fromUtf8("light_pos_y"));
        light_pos_y->setMinimum(-99);

        horizontalLayout_3->addWidget(light_pos_y);

        light_pos_z = new QDoubleSpinBox(groupBox_4);
        light_pos_z->setObjectName(QString::fromUtf8("light_pos_z"));
        light_pos_z->setMinimum(-99);

        horizontalLayout_3->addWidget(light_pos_z);


        formLayout_4->setLayout(1, QFormLayout::FieldRole, horizontalLayout_3);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_8);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        light_ambiant_x = new QDoubleSpinBox(groupBox_4);
        light_ambiant_x->setObjectName(QString::fromUtf8("light_ambiant_x"));

        horizontalLayout_19->addWidget(light_ambiant_x);

        light_ambiant_y = new QDoubleSpinBox(groupBox_4);
        light_ambiant_y->setObjectName(QString::fromUtf8("light_ambiant_y"));

        horizontalLayout_19->addWidget(light_ambiant_y);

        light_ambiant_z = new QDoubleSpinBox(groupBox_4);
        light_ambiant_z->setObjectName(QString::fromUtf8("light_ambiant_z"));

        horizontalLayout_19->addWidget(light_ambiant_z);


        formLayout_4->setLayout(2, QFormLayout::FieldRole, horizontalLayout_19);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, label_9);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        light_diffuse_x = new QDoubleSpinBox(groupBox_4);
        light_diffuse_x->setObjectName(QString::fromUtf8("light_diffuse_x"));

        horizontalLayout_20->addWidget(light_diffuse_x);

        light_diffuse_y = new QDoubleSpinBox(groupBox_4);
        light_diffuse_y->setObjectName(QString::fromUtf8("light_diffuse_y"));

        horizontalLayout_20->addWidget(light_diffuse_y);

        light_diffuse_z_2 = new QDoubleSpinBox(groupBox_4);
        light_diffuse_z_2->setObjectName(QString::fromUtf8("light_diffuse_z_2"));

        horizontalLayout_20->addWidget(light_diffuse_z_2);


        formLayout_4->setLayout(3, QFormLayout::FieldRole, horizontalLayout_20);

        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, label_10);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        light_specular_x = new QDoubleSpinBox(groupBox_4);
        light_specular_x->setObjectName(QString::fromUtf8("light_specular_x"));

        horizontalLayout_21->addWidget(light_specular_x);

        light_specular_y = new QDoubleSpinBox(groupBox_4);
        light_specular_y->setObjectName(QString::fromUtf8("light_specular_y"));

        horizontalLayout_21->addWidget(light_specular_y);

        light_specular_z = new QDoubleSpinBox(groupBox_4);
        light_specular_z->setObjectName(QString::fromUtf8("light_specular_z"));

        horizontalLayout_21->addWidget(light_specular_z);


        formLayout_4->setLayout(4, QFormLayout::FieldRole, horizontalLayout_21);

        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout_4->setWidget(5, QFormLayout::LabelRole, label_11);

        light_shininess = new QDoubleSpinBox(groupBox_4);
        light_shininess->setObjectName(QString::fromUtf8("light_shininess"));
        light_shininess->setSingleStep(0.1);

        formLayout_4->setWidget(5, QFormLayout::FieldRole, light_shininess);


        verticalLayout_5->addWidget(groupBox_4);

        groupBox_6 = new QGroupBox(tab_2);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setFlat(false);
        verticalLayout = new QVBoxLayout(groupBox_6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        light_scene_x = new QDoubleSpinBox(groupBox_6);
        light_scene_x->setObjectName(QString::fromUtf8("light_scene_x"));

        horizontalLayout_22->addWidget(light_scene_x);

        light_scene_y = new QDoubleSpinBox(groupBox_6);
        light_scene_y->setObjectName(QString::fromUtf8("light_scene_y"));

        horizontalLayout_22->addWidget(light_scene_y);

        light_scene_z = new QDoubleSpinBox(groupBox_6);
        light_scene_z->setObjectName(QString::fromUtf8("light_scene_z"));

        horizontalLayout_22->addWidget(light_scene_z);


        verticalLayout->addLayout(horizontalLayout_22);


        verticalLayout_5->addWidget(groupBox_6);

        node_type = new QTreeView(tab_2);
        node_type->setObjectName(QString::fromUtf8("node_type"));

        verticalLayout_5->addWidget(node_type);

        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));
        light_add = new QPushButton(tab_2);
        light_add->setObjectName(QString::fromUtf8("light_add"));

        horizontalLayout_26->addWidget(light_add);

        light_clone = new QPushButton(tab_2);
        light_clone->setObjectName(QString::fromUtf8("light_clone"));

        horizontalLayout_26->addWidget(light_clone);

        light_del = new QPushButton(tab_2);
        light_del->setObjectName(QString::fromUtf8("light_del"));

        horizontalLayout_26->addWidget(light_del);


        verticalLayout_5->addLayout(horizontalLayout_26);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_2 = new QVBoxLayout(tab_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        skybox_enable = new QGroupBox(tab_3);
        skybox_enable->setObjectName(QString::fromUtf8("skybox_enable"));
        skybox_enable->setEnabled(true);
        skybox_enable->setCheckable(true);
        skybox_enable->setChecked(false);
        formLayout_5 = new QFormLayout(skybox_enable);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        label_13 = new QLabel(skybox_enable);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_13);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        skybox_front = new QLineEdit(skybox_enable);
        skybox_front->setObjectName(QString::fromUtf8("skybox_front"));

        horizontalLayout_16->addWidget(skybox_front);

        skybox_front_browse = new QPushButton(skybox_enable);
        skybox_front_browse->setObjectName(QString::fromUtf8("skybox_front_browse"));
        sizePolicy1.setHeightForWidth(skybox_front_browse->sizePolicy().hasHeightForWidth());
        skybox_front_browse->setSizePolicy(sizePolicy1);

        horizontalLayout_16->addWidget(skybox_front_browse);

        horizontalLayout_16->setStretch(0, 6);
        horizontalLayout_16->setStretch(1, 1);

        formLayout_5->setLayout(0, QFormLayout::FieldRole, horizontalLayout_16);

        label_14 = new QLabel(skybox_enable);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        formLayout_5->setWidget(1, QFormLayout::LabelRole, label_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        skybox_back = new QLineEdit(skybox_enable);
        skybox_back->setObjectName(QString::fromUtf8("skybox_back"));

        horizontalLayout_15->addWidget(skybox_back);

        skybox_back_browse = new QPushButton(skybox_enable);
        skybox_back_browse->setObjectName(QString::fromUtf8("skybox_back_browse"));
        sizePolicy1.setHeightForWidth(skybox_back_browse->sizePolicy().hasHeightForWidth());
        skybox_back_browse->setSizePolicy(sizePolicy1);

        horizontalLayout_15->addWidget(skybox_back_browse);

        horizontalLayout_15->setStretch(0, 6);
        horizontalLayout_15->setStretch(1, 1);

        formLayout_5->setLayout(1, QFormLayout::FieldRole, horizontalLayout_15);

        label_22 = new QLabel(skybox_enable);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        formLayout_5->setWidget(2, QFormLayout::LabelRole, label_22);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        skybox_top = new QLineEdit(skybox_enable);
        skybox_top->setObjectName(QString::fromUtf8("skybox_top"));

        horizontalLayout_14->addWidget(skybox_top);

        skybox_top_browse = new QPushButton(skybox_enable);
        skybox_top_browse->setObjectName(QString::fromUtf8("skybox_top_browse"));
        sizePolicy1.setHeightForWidth(skybox_top_browse->sizePolicy().hasHeightForWidth());
        skybox_top_browse->setSizePolicy(sizePolicy1);

        horizontalLayout_14->addWidget(skybox_top_browse);

        horizontalLayout_14->setStretch(0, 6);
        horizontalLayout_14->setStretch(1, 1);

        formLayout_5->setLayout(2, QFormLayout::FieldRole, horizontalLayout_14);

        label_27 = new QLabel(skybox_enable);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        formLayout_5->setWidget(3, QFormLayout::LabelRole, label_27);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        skybox_bottom = new QLineEdit(skybox_enable);
        skybox_bottom->setObjectName(QString::fromUtf8("skybox_bottom"));

        horizontalLayout_4->addWidget(skybox_bottom);

        skybox_bottom_browse = new QPushButton(skybox_enable);
        skybox_bottom_browse->setObjectName(QString::fromUtf8("skybox_bottom_browse"));
        sizePolicy1.setHeightForWidth(skybox_bottom_browse->sizePolicy().hasHeightForWidth());
        skybox_bottom_browse->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(skybox_bottom_browse);

        horizontalLayout_4->setStretch(0, 6);
        horizontalLayout_4->setStretch(1, 1);

        formLayout_5->setLayout(3, QFormLayout::FieldRole, horizontalLayout_4);

        label_28 = new QLabel(skybox_enable);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        formLayout_5->setWidget(4, QFormLayout::LabelRole, label_28);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        skybox_left = new QLineEdit(skybox_enable);
        skybox_left->setObjectName(QString::fromUtf8("skybox_left"));

        horizontalLayout_6->addWidget(skybox_left);

        skybox_left_browse = new QPushButton(skybox_enable);
        skybox_left_browse->setObjectName(QString::fromUtf8("skybox_left_browse"));
        sizePolicy1.setHeightForWidth(skybox_left_browse->sizePolicy().hasHeightForWidth());
        skybox_left_browse->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(skybox_left_browse);

        horizontalLayout_6->setStretch(0, 6);
        horizontalLayout_6->setStretch(1, 1);

        formLayout_5->setLayout(4, QFormLayout::FieldRole, horizontalLayout_6);

        label_29 = new QLabel(skybox_enable);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        formLayout_5->setWidget(5, QFormLayout::LabelRole, label_29);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        skybox_right = new QLineEdit(skybox_enable);
        skybox_right->setObjectName(QString::fromUtf8("skybox_right"));

        horizontalLayout_5->addWidget(skybox_right);

        skybox_right_browse = new QPushButton(skybox_enable);
        skybox_right_browse->setObjectName(QString::fromUtf8("skybox_right_browse"));
        sizePolicy1.setHeightForWidth(skybox_right_browse->sizePolicy().hasHeightForWidth());
        skybox_right_browse->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(skybox_right_browse);

        horizontalLayout_5->setStretch(0, 6);
        horizontalLayout_5->setStretch(1, 1);

        formLayout_5->setLayout(5, QFormLayout::FieldRole, horizontalLayout_5);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        skybox_apply = new QPushButton(skybox_enable);
        skybox_apply->setObjectName(QString::fromUtf8("skybox_apply"));

        horizontalLayout_13->addWidget(skybox_apply);


        formLayout_5->setLayout(7, QFormLayout::SpanningRole, horizontalLayout_13);


        verticalLayout_2->addWidget(skybox_enable);

        fog_enable = new QGroupBox(tab_3);
        fog_enable->setObjectName(QString::fromUtf8("fog_enable"));
        fog_enable->setEnabled(true);
        fog_enable->setCheckable(true);
        fog_enable->setChecked(false);
        formLayout_6 = new QFormLayout(fog_enable);
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        formLayout_6->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_3 = new QLabel(fog_enable);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_3);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        fog_x = new QDoubleSpinBox(fog_enable);
        fog_x->setObjectName(QString::fromUtf8("fog_x"));

        horizontalLayout_17->addWidget(fog_x);

        fog_y = new QDoubleSpinBox(fog_enable);
        fog_y->setObjectName(QString::fromUtf8("fog_y"));

        horizontalLayout_17->addWidget(fog_y);

        fog_z = new QDoubleSpinBox(fog_enable);
        fog_z->setObjectName(QString::fromUtf8("fog_z"));

        horizontalLayout_17->addWidget(fog_z);


        formLayout_6->setLayout(0, QFormLayout::FieldRole, horizontalLayout_17);

        label_5 = new QLabel(fog_enable);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_6->setWidget(1, QFormLayout::LabelRole, label_5);

        fog_start = new QDoubleSpinBox(fog_enable);
        fog_start->setObjectName(QString::fromUtf8("fog_start"));
        fog_start->setSingleStep(0.1);

        formLayout_6->setWidget(1, QFormLayout::FieldRole, fog_start);

        label_4 = new QLabel(fog_enable);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_6->setWidget(2, QFormLayout::LabelRole, label_4);

        fog_end = new QDoubleSpinBox(fog_enable);
        fog_end->setObjectName(QString::fromUtf8("fog_end"));
        fog_end->setSingleStep(0.1);

        formLayout_6->setWidget(2, QFormLayout::FieldRole, fog_end);


        verticalLayout_2->addWidget(fog_enable);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_5);

        tabWidget->addTab(tab_3, QString());

        horizontalLayout_7->addWidget(tabWidget);

        horizontalLayout_7->setStretch(0, 8);
        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 779, 20));
        menuFichier = new QMenu(menubar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        mainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFichier->menuAction());
        menuFichier->addAction(actionOuvrire);
        menuFichier->addAction(actionEnregistrer);
        menuFichier->addAction(actionEnregistrer_sous);
        menuFichier->addSeparator();
        menuFichier->addAction(actionQuitter);

        retranslateUi(mainWindow);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QApplication::translate("mainWindow", "theBall Editor", 0, QApplication::UnicodeUTF8));
        actionOuvrire->setText(QApplication::translate("mainWindow", "Ouvrire", 0, QApplication::UnicodeUTF8));
        actionEnregistrer->setText(QApplication::translate("mainWindow", "Enregistrer", 0, QApplication::UnicodeUTF8));
        actionEnregistrer_sous->setText(QApplication::translate("mainWindow", "Enregistrer sous...", 0, QApplication::UnicodeUTF8));
        actionQuitter->setText(QApplication::translate("mainWindow", "Quitter", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("mainWindow", "Informations", 0, QApplication::UnicodeUTF8));
        infoText->setText(QString());
        groupBox->setTitle(QApplication::translate("mainWindow", "Propri\303\251t\303\251s", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("mainWindow", "Nom", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("mainWindow", "Position", 0, QApplication::UnicodeUTF8));
        node_mesh_add->setText(QApplication::translate("mainWindow", "Ajouter", 0, QApplication::UnicodeUTF8));
        node_mesh_clone->setText(QApplication::translate("mainWindow", "Cloner", 0, QApplication::UnicodeUTF8));
        node_mesh_del->setText(QApplication::translate("mainWindow", "Effacer", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("mainWindow", "Mesh", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("mainWindow", "Deform", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("mainWindow", "Size", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("mainWindow", "Uv repeat", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("mainWindow", "Speed", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("mainWindow", "Blend", 0, QApplication::UnicodeUTF8));
        node_water_add->setText(QApplication::translate("mainWindow", "Ajouter", 0, QApplication::UnicodeUTF8));
        node_water_clone->setText(QApplication::translate("mainWindow", "Cloner", 0, QApplication::UnicodeUTF8));
        node_water_del->setText(QApplication::translate("mainWindow", "Effacer", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("mainWindow", "Water", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("mainWindow", "Gravity", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("mainWindow", "FreeMove", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("mainWindow", "LifeInit", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("mainWindow", "LifeDown", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("mainWindow", "Number", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("mainWindow", "Texture", 0, QApplication::UnicodeUTF8));
        node_particles_texture_browse->setText(QApplication::translate("mainWindow", "...", 0, QApplication::UnicodeUTF8));
        node_particles_continousmide->setText(QApplication::translate("mainWindow", "Continous Mode", 0, QApplication::UnicodeUTF8));
        node_particles_build->setText(QApplication::translate("mainWindow", "B", 0, QApplication::UnicodeUTF8));
        node_particles_add->setText(QApplication::translate("mainWindow", "Ajouter", 0, QApplication::UnicodeUTF8));
        node_particles_clone->setText(QApplication::translate("mainWindow", "Cloner", 0, QApplication::UnicodeUTF8));
        node_particles_del->setText(QApplication::translate("mainWindow", "Effacer", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_6), QApplication::translate("mainWindow", "Particles", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("mainWindow", "Noeuds", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("mainWindow", "Propri\303\251t\303\251s", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("mainWindow", "Type", 0, QApplication::UnicodeUTF8));
        light_type->clear();
        light_type->insertItems(0, QStringList()
         << QApplication::translate("mainWindow", "Ponctuelle", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "Dirictionelle", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("mainWindow", "Position", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("mainWindow", "Ambient", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("mainWindow", "Diffuse", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("mainWindow", "Sp\303\251culaire", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("mainWindow", "Shininess", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("mainWindow", "Ambient de la scene", 0, QApplication::UnicodeUTF8));
        light_add->setText(QApplication::translate("mainWindow", "Ajouter", 0, QApplication::UnicodeUTF8));
        light_clone->setText(QApplication::translate("mainWindow", "Cloner", 0, QApplication::UnicodeUTF8));
        light_del->setText(QApplication::translate("mainWindow", "Effacer", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("mainWindow", "Lumiers", 0, QApplication::UnicodeUTF8));
        skybox_enable->setTitle(QApplication::translate("mainWindow", "Skybox", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("mainWindow", "Devant", 0, QApplication::UnicodeUTF8));
        skybox_front_browse->setText(QApplication::translate("mainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("mainWindow", "Deriere", 0, QApplication::UnicodeUTF8));
        skybox_back_browse->setText(QApplication::translate("mainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("mainWindow", "Haut", 0, QApplication::UnicodeUTF8));
        skybox_top_browse->setText(QApplication::translate("mainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("mainWindow", "Bas", 0, QApplication::UnicodeUTF8));
        skybox_bottom_browse->setText(QApplication::translate("mainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("mainWindow", "Gauche", 0, QApplication::UnicodeUTF8));
        skybox_left_browse->setText(QApplication::translate("mainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("mainWindow", "Droite", 0, QApplication::UnicodeUTF8));
        skybox_right_browse->setText(QApplication::translate("mainWindow", "...", 0, QApplication::UnicodeUTF8));
        skybox_apply->setText(QApplication::translate("mainWindow", "Apliquer", 0, QApplication::UnicodeUTF8));
        fog_enable->setTitle(QApplication::translate("mainWindow", "Fog", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("mainWindow", "Couleur", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("mainWindow", "D\303\251but", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("mainWindow", "Fin", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("mainWindow", "Environement", 0, QApplication::UnicodeUTF8));
        menuFichier->setTitle(QApplication::translate("mainWindow", "Fichier", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // INTERFACEXR2792_H
