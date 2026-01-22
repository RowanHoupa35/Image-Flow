#!/bin/bash

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║         ImageFlow - Push to GitHub                           ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""
echo "Repository: https://github.com/RowanHoupa35/Image-Flow"
echo ""
echo "You will need:"
echo "  • GitHub username: RowanHoupa35"
echo "  • Personal Access Token (NOT your password)"
echo ""
echo "Get token at: https://github.com/settings/tokens"
echo "  1. Generate new token (classic)"
echo "  2. Select scope: 'repo'"
echo "  3. Copy the token (ghp_...)"
echo ""
read -p "Press Enter when you have your token ready..."

echo ""
echo "Pushing to GitHub..."
echo ""

git push -u origin main

if [ $? -eq 0 ]; then
    echo ""
    echo "╔═══════════════════════════════════════════════════════════════╗"
    echo "║                   ✓ SUCCESS!                                 ║"
    echo "╠═══════════════════════════════════════════════════════════════╣"
    echo "║ ImageFlow pushed to GitHub!                                 ║"
    echo "║                                                              ║"
    echo "║ View at: https://github.com/RowanHoupa35/Image-Flow        ║"
    echo "╚═══════════════════════════════════════════════════════════════╝"
    echo ""
    echo "Next steps:"
    echo "  1. Visit your repository to verify"
    echo "  2. Add topics: cpp, openmp, sycl, image-processing"
    echo "  3. Share with your teacher!"
else
    echo ""
    echo "❌ Push failed. See PUSH_INSTRUCTIONS.md for help"
    echo ""
    echo "Common fix:"
    echo "  • Make sure you used a Personal Access Token (not password)"
    echo "  • Get token: https://github.com/settings/tokens"
fi
