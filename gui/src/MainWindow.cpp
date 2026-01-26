/**
 * @file MainWindow.cpp
 * @brief Implementation of the ImageFlow GUI main window
 *
 * This file implements all GUI functionality including:
 * - Window setup and layout construction
 * - Image loading/saving with Qt file dialogs
 * - Filter pipeline management (add, remove, reorder)
 * - Real-time preview with timer-based debouncing
 * - CPU/GPU processing mode selection
 *
 * @details
 * Event Handling:
 * - Uses Qt's signal/slot mechanism (SIGNAL/SLOT macros for Qt5 compat)
 * - Preview timer prevents excessive reprocessing during slider moves
 * - Progress callback updates QProgressBar during pipeline execution
 *
 * Filter Integration:
 * - Dynamically queries FilterFactory for available filters
 * - Builds filter menu at runtime (no hardcoded filter list)
 * - Only brightness/blur have parameter UI (design choice)
 *
 * Image Conversion:
 * - imageToPixmap(): Converts Image class to Qt's QPixmap
 * - Handles both RGB (3 channel) and grayscale (1 channel)
 * - Scales to 400x300 for display while preserving aspect ratio
 *
 * @see MainWindow.hpp for class declaration
 * @author Rowan HOUPA
 * @date January 2026
 */

#include "MainWindow.hpp"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QStandardPaths>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QToolBar>
#include <QLabel>
#include <QFrame>
#include <QCheckBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
    setWindowTitle("ImageFlow - Image Processing Application");
    setMinimumSize(1200, 700);
    
    createActions();
    createMenus();
    createToolbars();
    createStatusBar();
    createCentralWidget();
    
    previewTimer.setSingleShot(true);
    previewTimer.setInterval(100);
    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    
    showStatusMessage("Prêt. Chargez une image pour commencer.");
    
    qDebug() << "MainWindow créée avec succès";
}

MainWindow::~MainWindow() {
    qDebug() << "MainWindow détruite";
}

void MainWindow::createActions() {
    qDebug() << "Création des actions...";
    
    fileOpenAction = new QAction("&Ouvrir...", this);
    fileOpenAction->setShortcut(QKeySequence::Open);
    fileOpenAction->setStatusTip("Ouvrir une image");
    connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(onFileOpen()));
    qDebug() << "Action Open connectée";

    fileSaveAction = new QAction("&Enregistrer...", this);
    fileSaveAction->setShortcut(QKeySequence::Save);
    fileSaveAction->setStatusTip("Enregistrer l'image traitée");
    fileSaveAction->setEnabled(false);
    connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(onFileSave()));

    fileExitAction = new QAction("&Quitter", this);
    fileExitAction->setShortcut(QKeySequence::Quit);
    fileExitAction->setStatusTip("Quitter l'application");
    connect(fileExitAction, SIGNAL(triggered()), this, SLOT(onFileExit()));

    helpAboutAction = new QAction("&À propos", this);
    connect(helpAboutAction, SIGNAL(triggered()), this, SLOT(onAbout()));

    helpAboutQtAction = new QAction("À propos de &Qt", this);
    connect(helpAboutQtAction, SIGNAL(triggered()), this, SLOT(onAboutQt()));
}

void MainWindow::createMenus() {
    qDebug() << "Création des menus...";

    QMenu *fileMenu = menuBar()->addMenu("&Fichier");
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(fileExitAction);

    QMenu *helpMenu = menuBar()->addMenu("&Aide");
    helpMenu->addAction(helpAboutAction);
    helpMenu->addAction(helpAboutQtAction);
    
    qDebug() << "Menus créés";
}

void MainWindow::createToolbars() {
    QToolBar *toolbar = addToolBar("Principal");
    toolbar->addAction(fileOpenAction);
    toolbar->addAction(fileSaveAction);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Prêt");
}

void MainWindow::createCentralWidget() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QHBoxLayout(centralWidget);
    
    createImagePanel();
    createFilterPanel();
}

void MainWindow::createImagePanel() {
    QGroupBox *imageGroup = new QGroupBox("Images", centralWidget);
    imageLayout = new QVBoxLayout(imageGroup);
    
    // Image originale
    QLabel *originalTitle = new QLabel("Image Originale:");
    originalImageLabel = new QLabel();
    originalImageLabel->setAlignment(Qt::AlignCenter);
    originalImageLabel->setMinimumSize(400, 300);
    originalImageLabel->setFrameStyle(QFrame::Box);
    originalImageLabel->setText("Aucune image chargée");
    originalImageLabel->setStyleSheet("QLabel { background-color: #2d2d2d; color: #888; }");
    
    // Image traitée
    QLabel *processedTitle = new QLabel("Image Traitée:");
    processedImageLabel = new QLabel();
    processedImageLabel->setAlignment(Qt::AlignCenter);
    processedImageLabel->setMinimumSize(400, 300);
    processedImageLabel->setFrameStyle(QFrame::Box);
    processedImageLabel->setText("Chargez une image pour commencer");
    processedImageLabel->setStyleSheet("QLabel { background-color: #2d2d2d; color: #888; }");
    
    imageLayout->addWidget(originalTitle);
    imageLayout->addWidget(originalImageLabel);
    imageLayout->addSpacing(20);
    imageLayout->addWidget(processedTitle);
    imageLayout->addWidget(processedImageLabel);
    imageLayout->addStretch();
    
    mainLayout->addWidget(imageGroup, 2);
}

void MainWindow::createFilterPanel() {
    QGroupBox *controlGroup = new QGroupBox("Pipeline de Filtres", centralWidget);
    controlLayout = new QVBoxLayout(controlGroup);
    
    // Liste des filtres
    QLabel *filterListLabel = new QLabel("Filtres Actifs:");
    filterListWidget = new QListWidget();
    filterListWidget->setMinimumHeight(200);
    connect(filterListWidget, SIGNAL(currentRowChanged(int)),
            this, SLOT(onFilterSelected(int)));
    
    // Boutons de contrôle
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    addFilterButton = new QPushButton("Ajouter Filtre");
    removeFilterButton = new QPushButton("Retirer");
    moveUpButton = new QPushButton("▲");
    moveDownButton = new QPushButton("▼");
    clearButton = new QPushButton("Tout Effacer");
    
    moveUpButton->setFixedWidth(30);
    moveDownButton->setFixedWidth(30);

    connect(addFilterButton, SIGNAL(clicked()), this, SLOT(onAddFilter()));
    connect(removeFilterButton, SIGNAL(clicked()), this, SLOT(onRemoveFilter()));
    connect(moveUpButton, SIGNAL(clicked()), this, SLOT(onMoveFilterUp()));
    connect(moveDownButton, SIGNAL(clicked()), this, SLOT(onMoveFilterDown()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(onClearPipeline()));
    
    buttonLayout->addWidget(addFilterButton);
    buttonLayout->addWidget(removeFilterButton);
    buttonLayout->addWidget(moveUpButton);
    buttonLayout->addWidget(moveDownButton);
    buttonLayout->addWidget(clearButton);
    
    // Paramètres des filtres
    QGroupBox *paramGroup = new QGroupBox("Paramètres du Filtre");
    QFormLayout *paramLayout = new QFormLayout(paramGroup);
    
    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(0, 300);
    brightnessSlider->setValue(100);
    brightnessSlider->setTickPosition(QSlider::TicksBelow);
    brightnessSlider->setTickInterval(50);
    connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onBrightnessChanged(int)));

    blurRadiusSpinBox = new QSpinBox();
    blurRadiusSpinBox->setRange(1, 10);
    blurRadiusSpinBox->setValue(2);
    connect(blurRadiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onBlurRadiusChanged(int)));
    
    paramLayout->addRow("Luminosité (%):", brightnessSlider);
    paramLayout->addRow("Rayon du Flou:", blurRadiusSpinBox);
    
    // Preview and GPU
    previewCheckBox = new QCheckBox("Aperçu en temps réel");
    previewCheckBox->setChecked(true);
    connect(previewCheckBox, SIGNAL(toggled(bool)), this, SLOT(onPreviewToggle(bool)));

    gpuAccelerationCheckBox = new QCheckBox("Accélération GPU (SYCL)");
    gpuAccelerationCheckBox->setChecked(false);
    gpuAccelerationCheckBox->setToolTip("Utilise le GPU pour les filtres compatibles");
    connect(gpuAccelerationCheckBox, SIGNAL(toggled(bool)), this, SLOT(onGPUToggle(bool)));

    // Bouton Apply
    applyButton = new QPushButton("Appliquer le Pipeline");
    applyButton->setStyleSheet("QPushButton { background-color: #2d5aa0; color: white; padding: 8px; }");
    bool connected = connect(applyButton, SIGNAL(clicked()), this, SLOT(onApplyPipeline()));
    qDebug() << "Apply button connection:" << (connected ? "SUCCESS" : "FAILED");
    
    // Progress bar
    progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setVisible(false);
    
    // Assembly
    controlLayout->addWidget(filterListLabel);
    controlLayout->addWidget(filterListWidget);
    controlLayout->addLayout(buttonLayout);
    controlLayout->addWidget(paramGroup);
    controlLayout->addWidget(previewCheckBox);
    controlLayout->addWidget(gpuAccelerationCheckBox);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(applyButton);
    controlLayout->addWidget(progressBar);
    controlLayout->addStretch();
    
    mainLayout->addWidget(controlGroup, 1);
    
    // Désactiver les contrôles au départ
    setControlsEnabled(false);
}

// ==================== SLOTS FILE ====================

void MainWindow::onFileOpen() {
    qDebug() << "onFileOpen appelé !";
    
    QString filepath = QFileDialog::getOpenFileName(
        this,
        "Ouvrir une Image",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "Fichiers Image (*.png *.jpg *.jpeg *.bmp *.tga)"
    );
    
    qDebug() << "Fichier sélectionné:" << filepath;
    
    if (!filepath.isEmpty()) {
        loadImage(filepath);
    }
}

void MainWindow::onFileSave() {
    qDebug() << "========================================";
    qDebug() << "onFileSave CALLED!";
    qDebug() << "Processed image width:" << processedImage.getWidth();
    qDebug() << "Processed image height:" << processedImage.getHeight();
    qDebug() << "Save button enabled:" << fileSaveAction->isEnabled();
    qDebug() << "========================================";

    if (processedImage.getWidth() == 0) {
        qDebug() << "ERROR: No processed image to save!";
        QMessageBox::critical(this, "Erreur de Sauvegarde",
                            "Aucune image traitée à sauvegarder.\n\n"
                            "Veuillez:\n"
                            "1. Charger une image\n"
                            "2. Ajouter des filtres\n"
                            "3. Cliquer sur 'Appliquer le Pipeline'");
        return;
    }

    QString filepath = QFileDialog::getSaveFileName(
        this,
        "Enregistrer l'Image Traitée",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        "Images PNG (*.png);;Images JPEG (*.jpg *.jpeg);;Images BMP (*.bmp)"
    );

    qDebug() << "Selected save path:" << filepath;

    if (!filepath.isEmpty()) {
        qDebug() << "Attempting to save...";
        bool success = saveImage(filepath);
        qDebug() << "Save result:" << (success ? "SUCCESS" : "FAILED");
    } else {
        qDebug() << "Save cancelled by user";
    }
}

void MainWindow::onFileExit() {
    qDebug() << "onFileExit appelé !";
    close();
}

// ==================== GESTION DES FILTRES ====================

void MainWindow::onAddFilter() {
    qDebug() << "========================================";
    qDebug() << "onAddFilter appelé !";

    // Create menu dynamically from registered filters
    QMenu filterMenu(this);
    auto& factory = FilterFactory::instance();

    // Get all registered filters
    std::vector<std::string> filterIds = factory.getFilterIds();
    qDebug() << "Number of registered filters:" << filterIds.size();

    for (const auto& id : filterIds) {
        qDebug() << "  - Filter ID:" << QString::fromStdString(id);
    }

    std::map<QAction*, std::string> actionToId;

    // Build menu dynamically
    for (const auto& id : filterIds) {
        const auto* info = factory.getFilterInfo(id);
        if (info) {
            QString displayText = QString::fromStdString(info->name);
            if (info->hasGPUVersion && gpuEnabled) {
                displayText += " (GPU)";
            }
            QAction* action = filterMenu.addAction(displayText);
            actionToId[action] = id;
        }
    }

    // Show menu and get selection
    QAction* selected = filterMenu.exec(addFilterButton->mapToGlobal(
        QPoint(0, addFilterButton->height())
    ));

    if (!selected) {
        return; // User cancelled
    }

    // Create the selected filter using the factory
    std::string filterId = actionToId[selected];
    auto filter = factory.create(filterId, gpuEnabled);

    if (!filter) {
        qDebug() << "ERROR: Failed to create filter:" << QString::fromStdString(filterId);
        return;
    }

    // Handle special cases for filters with parameters we want to track
    // (This is the ONLY hardcoded part - for UI control only)
    if (filterId == "brightness") {
        BrightnessFilter* brightnessPtr = dynamic_cast<BrightnessFilter*>(filter.get());
        if (brightnessPtr) {
            brightnessFilter = brightnessPtr;
        }
    } else if (filterId == "boxblur" && !gpuEnabled) {
        BoxBlurFilter* blurPtr = dynamic_cast<BoxBlurFilter*>(filter.get());
        if (blurPtr) {
            blurFilter = blurPtr;
        }
    }

    qDebug() << "Filtre ajouté:" << QString::fromStdString(filter->getName());

    pipeline.addFilter(std::move(filter));
    updateFilterList();

    if (previewEnabled) {
        previewTimer.start();
    }
}

void MainWindow::onRemoveFilter() {
    int index = filterListWidget->currentRow();
    if (index >= 0 && index < static_cast<int>(pipeline.size())) {
        Filter* filter = pipeline.getFilter(index);
        if (filter == brightnessFilter) brightnessFilter = nullptr;
        if (filter == blurFilter) blurFilter = nullptr;
        
        pipeline.removeFilter(index);
        updateFilterList();
        
        if (previewEnabled) {
            previewTimer.start();
        }
    }
}

void MainWindow::onMoveFilterUp() {
    int index = filterListWidget->currentRow();
    if (index > 0) {
        pipeline.moveFilterUp(index);
        updateFilterList();
        filterListWidget->setCurrentRow(index - 1);
        
        if (previewEnabled) {
            previewTimer.start();
        }
    }
}

void MainWindow::onMoveFilterDown() {
    int index = filterListWidget->currentRow();
    if (index >= 0 && index < static_cast<int>(pipeline.size()) - 1) {
        pipeline.moveFilterDown(index);
        updateFilterList();
        filterListWidget->setCurrentRow(index + 1);
        
        if (previewEnabled) {
            previewTimer.start();
        }
    }
}

void MainWindow::onClearPipeline() {
    if (QMessageBox::question(this, "Effacer le Pipeline",
                              "Voulez-vous vraiment effacer tous les filtres ?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        pipeline.clear();
        brightnessFilter = nullptr;
        blurFilter = nullptr;
        updateFilterList();
        updateImageDisplays();
    }
}

void MainWindow::onFilterSelected(int index) {
    if (index >= 0 && index < static_cast<int>(pipeline.size())) {
        Filter* filter = pipeline.getFilter(index);
        
        brightnessSlider->setEnabled(dynamic_cast<BrightnessFilter*>(filter) != nullptr);
        blurRadiusSpinBox->setEnabled(dynamic_cast<BoxBlurFilter*>(filter) != nullptr);
    }
}

void MainWindow::onApplyPipeline() {
    qDebug() << "========================================";
    qDebug() << "onApplyPipeline CALLED!";
    qDebug() << "Image width:" << originalImage.getWidth();
    qDebug() << "Pipeline size:" << pipeline.size();
    qDebug() << "Button enabled:" << applyButton->isEnabled();
    qDebug() << "========================================";

    if (originalImage.getWidth() == 0) {
        qDebug() << "No image loaded!";
        QMessageBox::warning(this, "Aucune image",
                           "Veuillez d'abord charger une image avant d'appliquer des filtres.");
        return;
    }

    if (pipeline.empty()) {
        qDebug() << "Pipeline is empty!";
        QMessageBox::information(this, "Pipeline vide",
                               "Veuillez ajouter au moins un filtre au pipeline avant d'appliquer.");
        return;
    }

    qDebug() << "Starting applyFilters...";
    applyFilters(false);
    qDebug() << "Finished applyFilters!";
    showStatusMessage("Pipeline appliqué avec succès");
}

void MainWindow::onPreviewToggle(bool enabled) {
    previewEnabled = enabled;
    if (enabled && !pipeline.empty()) {
        previewTimer.start();
    }
}

void MainWindow::onGPUToggle(bool enabled) {
    gpuEnabled = enabled;
    qDebug() << "GPU acceleration" << (enabled ? "enabled" : "disabled");

    // Update pipeline processing mode
    if (enabled) {
        pipeline.setProcessingMode(FilterPipeline::ProcessingMode::GPU_PREFERRED);
    } else {
        pipeline.setProcessingMode(FilterPipeline::ProcessingMode::CPU_ONLY);
    }

    showStatusMessage(enabled ? "Mode GPU activé" : "Mode CPU activé");
}

void MainWindow::onBrightnessChanged(int value) {
    if (brightnessFilter) {
        brightnessFilter->setBrightness(value / 100.0f);
        if (previewEnabled) {
            previewTimer.start();
        }
    }
}

void MainWindow::onBlurRadiusChanged(int value) {
    if (blurFilter) {
        blurFilter->setRadius(value);
        if (previewEnabled) {
            previewTimer.start();
        }
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "À propos d'ImageFlow",
        "<h3>ImageFlow - Application de Traitement d'Images</h3>"
        "<p>Version 1.0.0</p>"
        "<p>Une application C++/Qt démontrant le traitement d'images extensible "
        "avec parallélisation CPU/GPU utilisant SYCL.</p>"
        "<p>Créée pour le cours de Programmation Orientée Objet & Génie Logiciel.</p>");
}

void MainWindow::onAboutQt() {
    QMessageBox::aboutQt(this);
}

// ==================== CORE FUNCTIONALITY ====================

bool MainWindow::loadImage(const QString &filepath) {
    qDebug() << "Chargement de l'image:" << filepath;
    showStatusMessage("Chargement de l'image...");
    
    Image newImage;
    if (!newImage.loadFromFile(filepath.toStdString())) {
        showErrorMessage("Erreur de Chargement", "Impossible de charger l'image: " + filepath);
        return false;
    }
    
    originalImage = std::move(newImage);
    currentImagePath = filepath;
    
    // Reset pipeline
    pipeline.clear();
    brightnessFilter = nullptr;
    blurFilter = nullptr;
    
    // Enable controls
    setControlsEnabled(true);
    fileSaveAction->setEnabled(true);
    
    updateImageDisplays();
    updateFilterList();
    
    showStatusMessage("Chargé: " + QFileInfo(filepath).fileName() +
                     " (" + QString::number(originalImage.getWidth()) + "x" +
                     QString::number(originalImage.getHeight()) + ")");
    
    qDebug() << "Image chargée avec succès";
    return true;
}

bool MainWindow::saveImage(const QString &filepath) {
    if (processedImage.getWidth() == 0) {
        showErrorMessage("Erreur de Sauvegarde", "Aucune image traitée à sauvegarder");
        return false;
    }
    
    showStatusMessage("Enregistrement de l'image...");
    
    if (!processedImage.saveToFile(filepath.toStdString())) {
        showErrorMessage("Erreur de Sauvegarde", "Impossible d'enregistrer l'image: " + filepath);
        return false;
    }
    
    showStatusMessage("Enregistré: " + QFileInfo(filepath).fileName());
    return true;
}

void MainWindow::updateImageDisplays() {
    if (originalImage.getWidth() == 0) {
        originalImageLabel->setText("Aucune image chargée");
        processedImageLabel->setText("Chargez une image pour commencer");
        return;
    }
    
    originalImageLabel->setPixmap(imageToPixmap(originalImage));
    
    if (processedImage.getWidth() > 0) {
        processedImageLabel->setPixmap(imageToPixmap(processedImage));
    } else {
        processedImageLabel->setPixmap(imageToPixmap(originalImage));
    }
}

void MainWindow::updateFilterList() {
    filterListWidget->clear();
    
    for (size_t i = 0; i < pipeline.size(); ++i) {
        const Filter* filter = pipeline.getFilter(i);
        if (filter) {
            QListWidgetItem *item = new QListWidgetItem(
                QString::fromStdString(filter->getName())
            );
            filterListWidget->addItem(item);
        }
    }
    
    removeFilterButton->setEnabled(!pipeline.empty());
    moveUpButton->setEnabled(!pipeline.empty());
    moveDownButton->setEnabled(!pipeline.empty());
    clearButton->setEnabled(!pipeline.empty());
}

void MainWindow::updatePreview() {
    if (!previewEnabled || originalImage.getWidth() == 0 || pipeline.empty()) {
        return;
    }
    
    applyFilters(true);
}

void MainWindow::applyFilters(bool preview) {
    if (originalImage.getWidth() == 0 || pipeline.empty()) {
        return;
    }

    try {
        if (!preview && !isProcessing) {
            // Show progress bar for full processing
            isProcessing = true;
            progressBar->setVisible(true);
            progressBar->setValue(0);
            applyButton->setEnabled(false);

            // Apply with progress callback
            processedImage = pipeline.applyWithProgress(originalImage,
                [this](float percent, const std::string& filterName) {
                    progressBar->setValue(static_cast<int>(percent));
                    showStatusMessage("Traitement: " + QString::fromStdString(filterName) +
                                    " (" + QString::number(static_cast<int>(percent)) + "%)", 0);
                    QApplication::processEvents(); // Update UI
                });

            progressBar->setValue(100);
            progressBar->setVisible(false);
            applyButton->setEnabled(true);
            isProcessing = false;
        } else {
            // Quick preview without progress bar
            processedImage = pipeline.apply(originalImage);
        }

        updateImageDisplays();
    } catch (const std::exception &e) {
        progressBar->setVisible(false);
        applyButton->setEnabled(true);
        isProcessing = false;
        showErrorMessage("Erreur de Traitement", QString::fromStdString(e.what()));
    }
}

QPixmap MainWindow::imageToPixmap(const Image &img) const {
    if (img.getWidth() == 0 || img.getHeight() == 0) {
        return QPixmap();
    }
    
    QImage qimg(img.getWidth(), img.getHeight(), QImage::Format_RGB888);
    
    if (img.getChannels() == 3) {
        for (int y = 0; y < img.getHeight(); ++y) {
            for (int x = 0; x < img.getWidth(); ++x) {
                int r = img.at(x, y, 0);
                int g = img.at(x, y, 1);
                int b = img.at(x, y, 2);
                qimg.setPixel(x, y, qRgb(r, g, b));
            }
        }
    } else if (img.getChannels() == 1) {
        for (int y = 0; y < img.getHeight(); ++y) {
            for (int x = 0; x < img.getWidth(); ++x) {
                int gray = img.at(x, y, 0);
                qimg.setPixel(x, y, qRgb(gray, gray, gray));
            }
        }
    }
    
    QPixmap pixmap = QPixmap::fromImage(qimg);
    return pixmap.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void MainWindow::setControlsEnabled(bool enabled) {
    addFilterButton->setEnabled(enabled);
    removeFilterButton->setEnabled(enabled && !pipeline.empty());
    moveUpButton->setEnabled(enabled && !pipeline.empty());
    moveDownButton->setEnabled(enabled && !pipeline.empty());
    clearButton->setEnabled(enabled && !pipeline.empty());
    applyButton->setEnabled(enabled && !pipeline.empty());
    previewCheckBox->setEnabled(enabled);
    gpuAccelerationCheckBox->setEnabled(enabled);
    brightnessSlider->setEnabled(enabled);
    blurRadiusSpinBox->setEnabled(enabled);
}

void MainWindow::showStatusMessage(const QString &message, int timeout) {
    statusBar()->showMessage(message, timeout);
}

void MainWindow::showErrorMessage(const QString &title, const QString &message) {
    QMessageBox::critical(this, title, message);
    showStatusMessage("Erreur: " + message);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}
