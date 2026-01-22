#!/bin/bash

# Script to push ImageFlow to GitHub

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║         Push ImageFlow to GitHub                             ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""

# Check if GitHub repository URL is provided
if [ -z "$1" ]; then
    echo "❌ Error: GitHub repository URL required"
    echo ""
    echo "Usage: ./push_to_github.sh <your-github-repo-url>"
    echo ""
    echo "Examples:"
    echo "  ./push_to_github.sh https://github.com/YourUsername/ImageFlow.git"
    echo "  ./push_to_github.sh git@github.com:YourUsername/ImageFlow.git"
    echo ""
    echo "Steps to create a GitHub repository:"
    echo "  1. Go to https://github.com/new"
    echo "  2. Repository name: ImageFlow"
    echo "  3. Description: Image Processing with CPU/GPU Parallelization"
    echo "  4. Choose Public or Private"
    echo "  5. Do NOT initialize with README (we already have one)"
    echo "  6. Click 'Create repository'"
    echo "  7. Copy the repository URL"
    echo "  8. Run: ./push_to_github.sh <copied-url>"
    exit 1
fi

REPO_URL="$1"

echo "Repository URL: $REPO_URL"
echo ""

# Check if remote already exists
if git remote get-url origin &> /dev/null; then
    echo "⚠️  Remote 'origin' already exists"
    echo "Current remote: $(git remote get-url origin)"
    read -p "Do you want to update it? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        git remote set-url origin "$REPO_URL"
        echo "✓ Remote updated"
    else
        echo "Keeping existing remote"
    fi
else
    # Add remote
    echo "Adding remote 'origin'..."
    git remote add origin "$REPO_URL"
    echo "✓ Remote added"
fi

echo ""

# Show current branch and status
echo "Current branch: $(git branch --show-current)"
echo "Files in commit: $(git log --oneline -1)"
echo ""

# Push to GitHub
echo "Pushing to GitHub..."
echo ""

if git push -u origin main; then
    echo ""
    echo "╔═══════════════════════════════════════════════════════════════╗"
    echo "║                   ✓ SUCCESS!                                 ║"
    echo "╠═══════════════════════════════════════════════════════════════╣"
    echo "║ ImageFlow has been pushed to GitHub                         ║"
    echo "║                                                              ║"
    echo "║ View your repository at:                                    ║"
    echo "║ $(printf '%-60s' "$REPO_URL" | sed 's/.git$//')                                         ║"
    echo "╚═══════════════════════════════════════════════════════════════╝"
    echo ""
    echo "Next steps:"
    echo "  • Add a repository description on GitHub"
    echo "  • Add topics: cpp, openmp, sycl, image-processing, qt6"
    echo "  • Consider adding a LICENSE file"
    echo "  • Share the repository link with your teacher!"
else
    echo ""
    echo "❌ Push failed!"
    echo ""
    echo "Common issues:"
    echo "  1. Authentication: Make sure you're logged in to GitHub"
    echo "     - For HTTPS: git config --global credential.helper store"
    echo "     - For SSH: Add your SSH key to GitHub"
    echo ""
    echo "  2. Wrong URL: Verify the repository URL is correct"
    echo ""
    echo "  3. Repository doesn't exist: Create it on GitHub first"
    echo "     Visit: https://github.com/new"
    exit 1
fi
