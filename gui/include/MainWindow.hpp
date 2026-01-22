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

// ... rest of the class definition stays the same

/**
 * @class MainWindow
 * @brief Main application window for ImageFlow.
 * 
 * Provides a graphical interface to load images, apply filters,
 * and manage filter pipelines with real-time preview.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // File operations
    void onFileOpen();
    void onFileSave();
    void onFileExit();
    
    // Filter management
    void onAddFilter();
    void onRemoveFilter();
    void onMoveFilterUp();
    void onMoveFilterDown();
    void onClearPipeline();
    void onFilterSelected(int index);
    
    // Processing
    void onApplyPipeline();
    void onPreviewToggle(bool enabled);
    void onGPUToggle(bool enabled);
    
    // Filter parameters
    void onBrightnessChanged(int value);
    void onBlurRadiusChanged(int value);

    // Preview timer
    void updatePreview();

    // Help
    void onAbout();
    void onAboutQt();

private:
    // UI Setup
    void createActions();
    void createMenus();
    void createToolbars();
    void createStatusBar();
    void createCentralWidget();
    void createFilterPanel();
    void createImagePanel();
    
    // Core functionality
    bool loadImage(const QString &filepath);
    bool saveImage(const QString &filepath);
    void updateImageDisplays();
    void updateFilterList();
    void applyFilters(bool preview = false);
    
    // Utility
    QPixmap imageToPixmap(const Image &img) const;
    void showStatusMessage(const QString &message, int timeout = 5000);
    void showErrorMessage(const QString &title, const QString &message);
    void setControlsEnabled(bool enabled);
    
    // Data members
    Image originalImage;
    Image processedImage;
    FilterPipeline pipeline;
    
    // Filter instances for UI (owned by pipeline, but we keep references)
    BrightnessFilter* brightnessFilter = nullptr;
    BoxBlurFilter* blurFilter = nullptr;
    
    // UI Components (we'll create them manually, not with .ui file for clarity)
    QLabel *originalImageLabel;
    QLabel *processedImageLabel;
    QListWidget *filterListWidget;
    
    // Controls
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
    
    // Layouts
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *imageLayout;
    QVBoxLayout *controlLayout;
    
    // Menu actions
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileExitAction;
    QAction *helpAboutAction;
    QAction *helpAboutQtAction;
    
    // State
    QString currentImagePath;
    bool previewEnabled = true;
    bool gpuEnabled = false;
    bool isProcessing = false;

    // Preview timer for real-time updates
    QTimer previewTimer;
};

#endif // MAINWINDOW_HPP