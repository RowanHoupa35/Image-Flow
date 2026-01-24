/**
 * @file imageflow_cli.cpp
 * @brief Command-line interface for ImageFlow image processing
 *
 * Provides a terminal-based interface for batch image processing without
 * requiring the GUI. Supports single image, batch, and interactive modes.
 *
 * @details
 * Commands:
 * - list: Show all image files in current directory
 * - process <file>: Interactive filter selection for single image
 * - batch: Apply same pipeline to all images in directory
 * - help: Display usage information
 *
 * Features:
 * - ANSI color output for better terminal readability
 * - Dynamic filter discovery from FilterFactory
 * - Supports both CPU and GPU filter variants
 * - Parameter prompts for configurable filters (brightness, blur)
 * - Timing information for performance analysis
 *
 * Filter Selection:
 * - Queries FilterFactory at runtime for available filters
 * - Lists GPU variants separately when available
 * - Prompts for parameters where needed (brightness factor, blur radius)
 *
 * Output Naming:
 * - Single image: <name>_processed.<ext>
 * - Batch mode: <name>_batch.<ext>
 *
 * @see FilterFactory for filter registration system
 * @author Rowan HOUPA
 * @date January 2026
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <memory>

#include "Image.hpp"
#include "FilterPipeline.hpp"
#include "FilterFactory.hpp"
#include "filters/BrightnessFilter.hpp"  // For parameter input only
#include "filters/BoxBlurFilter.hpp"     // For parameter input only

namespace fs = std::filesystem;

// Couleurs ANSI pour terminal
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

void printHeader() {
    std::cout << CYAN << BOLD;
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    IMAGEFLOW CLI                              ║\n";
    std::cout << "║          Traitement d'Images avec CPU/GPU                    ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n";
    std::cout << RESET << "\n";
}

void printHelp() {
    std::cout << BOLD << "UTILISATION:\n" << RESET;
    std::cout << "  imageflow_cli <commande> [options]\n\n";

    std::cout << BOLD << "COMMANDES:\n" << RESET;
    std::cout << "  " << GREEN << "list" << RESET << "                Liste les images dans le dossier\n";
    std::cout << "  " << GREEN << "process" << RESET << " <image>     Traiter une image spécifique\n";
    std::cout << "  " << GREEN << "batch" << RESET << "               Traiter toutes les images du dossier\n";
    std::cout << "  " << GREEN << "help" << RESET << "                Afficher cette aide\n\n";

    std::cout << BOLD << "FILTRES DISPONIBLES:\n" << RESET;

    // Dynamically list filters from FilterFactory
    auto& factory = FilterFactory::instance();
    auto filterIds = factory.getFilterIds();

    int index = 1;
    for (const auto& id : filterIds) {
        auto info = factory.getFilterInfo(id);
        if (info) {
            std::cout << "  " << std::setw(2) << index << ". " << YELLOW << id << RESET
                      << " - " << info->description << " (CPU)\n";
            index++;

            // If GPU version exists, list it too
            if (info->hasGPUVersion) {
                std::cout << "  " << std::setw(2) << index << ". " << YELLOW << id << "-gpu" << RESET
                          << " - " << info->description << " (GPU SYCL)\n";
                index++;
            }
        }
    }
    std::cout << "\n";

    std::cout << BOLD << "EXEMPLES:\n" << RESET;
    std::cout << "  imageflow_cli list\n";
    std::cout << "  imageflow_cli process photo.jpg\n";
    std::cout << "  imageflow_cli batch\n\n";
}

std::vector<std::string> listImages(const std::string& directory = ".") {
    std::vector<std::string> images;
    
    if (!fs::exists(directory)) {
        std::cerr << RED << "Erreur: Le dossier n'existe pas: " << directory << RESET << "\n";
        return images;
    }
    
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            
            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || 
                ext == ".bmp" || ext == ".tga") {
                images.push_back(entry.path().filename().string());
            }
        }
    }
    
    std::sort(images.begin(), images.end());
    return images;
}

void displayImageList(const std::vector<std::string>& images) {
    if (images.empty()) {
        std::cout << YELLOW << "Aucune image trouvée dans le dossier actuel.\n" << RESET;
        return;
    }
    
    std::cout << BOLD << "Images disponibles (" << images.size() << "):\n" << RESET;
    std::cout << std::string(60, '-') << "\n";
    
    for (size_t i = 0; i < images.size(); ++i) {
        std::cout << std::setw(3) << (i + 1) << ". " << images[i] << "\n";
    }
    std::cout << std::string(60, '-') << "\n";
}

int selectFilter() {
    std::cout << "\n" << BOLD << "Sélectionnez un filtre:\n" << RESET;

    // Dynamically list filters from FilterFactory
    auto& factory = FilterFactory::instance();
    auto filterIds = factory.getFilterIds();

    int index = 1;
    for (const auto& id : filterIds) {
        auto info = factory.getFilterInfo(id);
        if (info) {
            std::cout << "  " << std::setw(2) << index << ". " << info->name << " (CPU)\n";
            index++;

            // If GPU version exists, list it too
            if (info->hasGPUVersion) {
                std::cout << "  " << std::setw(2) << index << ". " << info->name << " (GPU SYCL)\n";
                index++;
            }
        }
    }

    std::cout << "  0. Terminer\n";
    std::cout << "\nChoix: ";

    int choice;
    std::cin >> choice;
    return choice;
}

std::unique_ptr<Filter> createFilter(int choice) {
    if (choice <= 0) {
        return nullptr;
    }

    auto& factory = FilterFactory::instance();
    auto filterIds = factory.getFilterIds();

    // Map choice to filter ID and GPU flag
    int index = 1;
    std::string selectedId;
    bool useGPU = false;

    for (const auto& id : filterIds) {
        auto info = factory.getFilterInfo(id);
        if (!info) continue;

        // CPU version
        if (index == choice) {
            selectedId = id;
            useGPU = false;
            break;
        }
        index++;

        // GPU version if available
        if (info->hasGPUVersion) {
            if (index == choice) {
                selectedId = id;
                useGPU = true;
                break;
            }
            index++;
        }
    }

    if (selectedId.empty()) {
        return nullptr;
    }

    // Handle filters that need parameters
    std::unique_ptr<Filter> filter;

    if (selectedId == "brightness") {
        std::cout << "Facteur de luminosité (0.5 = sombre, 1.0 = normal, 2.0 = clair): ";
        float factor;
        std::cin >> factor;
        filter = std::make_unique<BrightnessFilter>(factor);
    }
    else if (selectedId == "blur") {
        std::cout << "Rayon du flou (1-10): ";
        int radius;
        std::cin >> radius;
        if (useGPU) {
            filter = factory.create(selectedId, true);
            // Set radius via dynamic cast (not ideal but works for now)
            auto* blurFilter = dynamic_cast<BoxBlurFilter*>(filter.get());
            if (blurFilter) {
                *blurFilter = BoxBlurFilter(radius);
            }
        } else {
            filter = std::make_unique<BoxBlurFilter>(radius);
        }
    }
    else {
        // No parameters needed - use factory directly
        filter = factory.create(selectedId, useGPU);
    }

    return filter;
}

bool processImage(const std::string& inputPath, FilterPipeline& pipeline, const std::string& outputSuffix = "_processed") {
    std::cout << "\n" << CYAN << "Traitement de: " << inputPath << RESET << "\n";
    
    // Charger l'image
    Image input;
    if (!input.loadFromFile(inputPath)) {
        std::cerr << RED << "Erreur: Impossible de charger " << inputPath << RESET << "\n";
        return false;
    }
    
    std::cout << GREEN << "✓" << RESET << " Image chargée: " 
              << input.getWidth() << "x" << input.getHeight() 
              << " (" << input.getChannels() << " canaux)\n";
    
    // Appliquer le pipeline
    std::cout << YELLOW << "⚙ Application de " << pipeline.size() << " filtre(s)...\n" << RESET;
    
    auto start = std::chrono::high_resolution_clock::now();
    Image output = pipeline.apply(input);
    auto end = std::chrono::high_resolution_clock::now();
    
    double duration = std::chrono::duration<double, std::milli>(end - start).count();
    
    std::cout << GREEN << "✓" << RESET << " Traitement terminé en " 
              << std::fixed << std::setprecision(2) << duration << " ms\n";
    
    // Générer le nom du fichier de sortie
    fs::path inputPathFs(inputPath);
    std::string outputPath = inputPathFs.stem().string() + outputSuffix + inputPathFs.extension().string();
    
    // Sauvegarder
    if (!output.saveToFile(outputPath)) {
        std::cerr << RED << "Erreur: Impossible de sauvegarder " << outputPath << RESET << "\n";
        return false;
    }
    
    std::cout << GREEN << "✓" << RESET << " Sauvegardé: " << BOLD << outputPath << RESET << "\n";
    return true;
}

void interactiveMode(const std::string& imagePath) {
    Image input;
    if (!input.loadFromFile(imagePath)) {
        std::cerr << RED << "Erreur: Impossible de charger " << imagePath << RESET << "\n";
        return;
    }
    
    std::cout << GREEN << "\n✓ Image chargée: " << imagePath << RESET << "\n";
    std::cout << "  Dimensions: " << input.getWidth() << "x" << input.getHeight() << "\n";
    std::cout << "  Canaux: " << input.getChannels() << "\n";
    
    FilterPipeline pipeline;
    
    while (true) {
        int choice = selectFilter();
        
        if (choice == 0) break;
        
        auto filter = createFilter(choice);
        if (filter) {
            pipeline.addFilter(std::move(filter));
            std::cout << GREEN << "✓ Filtre ajouté au pipeline\n" << RESET;
            std::cout << "Pipeline actuel: " << pipeline.getDescription() << "\n";
        } else {
            std::cout << RED << "Choix invalide\n" << RESET;
        }
    }
    
    if (pipeline.empty()) {
        std::cout << YELLOW << "Aucun filtre sélectionné. Abandon.\n" << RESET;
        return;
    }
    
    // Traiter
    processImage(imagePath, pipeline);
}

void batchMode() {
    auto images = listImages();
    
    if (images.empty()) {
        std::cout << YELLOW << "Aucune image trouvée.\n" << RESET;
        return;
    }
    
    displayImageList(images);
    
    // Construire le pipeline
    FilterPipeline pipeline;
    std::cout << "\n" << BOLD << "Construction du pipeline pour le traitement batch:\n" << RESET;
    
    while (true) {
        int choice = selectFilter();
        if (choice == 0) break;
        
        auto filter = createFilter(choice);
        if (filter) {
            pipeline.addFilter(std::move(filter));
            std::cout << GREEN << "✓ Filtre ajouté\n" << RESET;
        }
    }
    
    if (pipeline.empty()) {
        std::cout << YELLOW << "Aucun filtre sélectionné. Abandon.\n" << RESET;
        return;
    }
    
    std::cout << "\n" << BOLD << "Pipeline: " << pipeline.getDescription() << RESET << "\n";
    std::cout << "\n" << CYAN << "Traitement de " << images.size() << " image(s)...\n" << RESET;
    std::cout << std::string(60, '=') << "\n";
    
    int success = 0;
    int failed = 0;
    
    for (const auto& img : images) {
        if (processImage(img, pipeline, "_batch")) {
            success++;
        } else {
            failed++;
        }
        std::cout << std::string(60, '-') << "\n";
    }
    
    std::cout << std::string(60, '=') << "\n";
    std::cout << BOLD << "RÉSUMÉ:\n" << RESET;
    std::cout << GREEN << "  Réussis: " << success << RESET << "\n";
    if (failed > 0) {
        std::cout << RED << "  Échoués: " << failed << RESET << "\n";
    }
}

// Forward declaration for filter registration
extern void registerAllFilters();

int main(int argc, char* argv[]) {
    // CRITICAL: Initialize filters before using CLI
    std::cout << CYAN << "Initialisation des filtres...\n" << RESET;
    registerAllFilters();

    auto& factory = FilterFactory::instance();
    std::cout << GREEN << "✓ " << factory.getFilterIds().size() << " filtres disponibles\n" << RESET;

    printHeader();

    if (argc < 2) {
        printHelp();
        return 0;
    }

    std::string command = argv[1];

    if (command == "help" || command == "--help" || command == "-h") {
        printHelp();
    }
    else if (command == "list") {
        auto images = listImages();
        displayImageList(images);
    }
    else if (command == "process") {
        if (argc < 3) {
            std::cerr << RED << "Erreur: Nom de fichier manquant\n" << RESET;
            std::cout << "Usage: imageflow_cli process <image.jpg>\n";
            return 1;
        }
        interactiveMode(argv[2]);
    }
    else if (command == "batch") {
        batchMode();
    }
    else {
        std::cerr << RED << "Commande inconnue: " << command << RESET << "\n";
        printHelp();
        return 1;
    }

    return 0;
}