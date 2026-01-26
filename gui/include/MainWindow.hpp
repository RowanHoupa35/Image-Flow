/**
 * @file MainWindow.hpp
 * @brief Main GUI window for the ImageFlow application
 *
 * This file defines the MainWindow class which provides the graphical user
 * interface for ImageFlow. It integrates Qt5 widgets with the core image
 * processing pipeline.
 *
 * @details
 * GUI Architecture:
 * - Left panel: Original and processed image display (QLabel with QPixmap)
 * - Right panel: Filter pipeline management, parameters, GPU toggle
 * - Menu bar: File operations (open, save, exit), Help (about)
 * - Status bar: Current operation status and image info
 *
 * Key Features:
 * - Real-time preview with debounced timer (100ms delay)
 * - Dynamic filter menu built from FilterFactory registry
 * - GPU acceleration toggle (SYCL-based)
 * - Progress bar for long-running pipeline operations
 * - Parameter sliders for brightness and blur radius
 *
 * Qt Components Used:
 * - QMainWindow: Main application window framework
 * - QListWidget: Filter pipeline display and selection
 * - QSlider/QSpinBox: Filter parameter controls
 * - QTimer: Debounced preview updates
 * - Signals/Slots: Qt's event-driven programming model
 *
 * @see MainWindow.cpp for implementation
 * @author Rowan HOUPA
 * @date January 2026
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QProgressBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QPixmap>
#include <QImage>
#include <QTimer>
#include <QCheckBox>
#include <QMenu>
#include <QFileInfo>
#include <QStandardPaths>
#include <QCloseEvent>

#include "Image.hpp"
#include "FilterPipeline.hpp"
#include "FilterFactory.hpp"
#include "filters/BrightnessFilter.hpp"
#include "filters/BoxBlurFilter.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onFileOpen();
    void onFileSave();
    void onFileExit();

    void onAddFilter();
    void onRemoveFilter();
    void onMoveFilterUp();
    void onMoveFilterDown();
    void onClearPipeline();
    void onFilterSelected(int index);

    void onApplyPipeline();
    void onPreviewToggle(bool enabled);
    void onGPUToggle(bool enabled);

    void onBrightnessChanged(int value);
    void onBlurRadiusChanged(int value);

    void updatePreview();

    void onAbout();
    void onAboutQt();

private:
    void createActions();
    void createMenus();
    void createToolbars();
    void createStatusBar();
    void createCentralWidget();
    void createFilterPanel();
    void createImagePanel();

    bool loadImage(const QString &filepath);
    bool saveImage(const QString &filepath);
    void updateImageDisplays();
    void updateFilterList();
    void applyFilters(bool preview = false);

    QPixmap imageToPixmap(const Image &img) const;
    void showStatusMessage(const QString &message, int timeout = 5000);
    void showErrorMessage(const QString &title, const QString &message);
    void setControlsEnabled(bool enabled);
    
    Image originalImage;
    Image processedImage;
    FilterPipeline pipeline;
    
    BrightnessFilter* brightnessFilter = nullptr;
    BoxBlurFilter* blurFilter = nullptr;
    
    QLabel *originalImageLabel;
    QLabel *processedImageLabel;
    QListWidget *filterListWidget;
    
    QPushButton *addFilterButton;
    QPushButton *removeFilterButton;
    QPushButton *moveUpButton;
    QPushButton *moveDownButton;
    QPushButton *applyButton;
    QPushButton *clearButton;
    
    QSlider *brightnessSlider;
    QSpinBox *blurRadiusSpinBox;
    QCheckBox *previewCheckBox;
    QCheckBox *gpuAccelerationCheckBox;
    QProgressBar *progressBar;
    
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *imageLayout;
    QVBoxLayout *controlLayout;
    
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileExitAction;
    QAction *helpAboutAction;
    QAction *helpAboutQtAction;
    
    QString currentImagePath;
    bool previewEnabled = true;
    bool gpuEnabled = false;
    bool isProcessing = false;

    QTimer previewTimer;
};

#endif
