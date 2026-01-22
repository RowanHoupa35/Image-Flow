# GitHub Push Guide for ImageFlow

## Quick Start

### Step 1: Create GitHub Repository

1. Go to https://github.com/new
2. Fill in:
   - **Repository name:** `ImageFlow`
   - **Description:** `Image Processing with CPU/GPU Parallelization - Demonstrating Polymorphism, OpenMP, and SYCL`
   - **Visibility:** Choose Public or Private
   - **Important:** Do NOT check "Initialize with README" (we already have one)
3. Click **"Create repository"**
4. Copy the repository URL (e.g., `https://github.com/YourUsername/ImageFlow.git`)

### Step 2: Push to GitHub

Run the push script with your repository URL:

```bash
./push_to_github.sh https://github.com/YourUsername/ImageFlow.git
```

**Or manually:**

```bash
# Add remote
git remote add origin https://github.com/YourUsername/ImageFlow.git

# Push to GitHub
git push -u origin main
```

### Step 3: Verify

1. Go to your GitHub repository page
2. You should see all files including README.md
3. The README will be displayed on the repository homepage

## Authentication Setup

### Option 1: HTTPS (Easier)

If you get prompted for username/password:

```bash
# Store credentials (will ask once, then remember)
git config --global credential.helper store
git push -u origin main
# Enter your GitHub username and Personal Access Token
```

**Get Personal Access Token:**
1. GitHub Settings → Developer settings → Personal access tokens → Tokens (classic)
2. Generate new token
3. Select scopes: `repo` (full control of private repositories)
4. Copy the token (you won't see it again!)
5. Use the token as your password when pushing

### Option 2: SSH (More Secure)

```bash
# Generate SSH key (if you don't have one)
ssh-keygen -t ed25519 -C "houparowan@gmail.com"

# Copy public key
cat ~/.ssh/id_ed25519.pub

# Add to GitHub:
# 1. GitHub Settings → SSH and GPG keys → New SSH key
# 2. Paste the key
# 3. Save

# Use SSH URL instead of HTTPS
git remote set-url origin git@github.com:YourUsername/ImageFlow.git
git push -u origin main
```

## After Pushing

### Add Repository Details

1. **Add Topics** (helps people find your project):
   - Click ⚙️ next to "About" on repository page
   - Add topics: `cpp`, `cpp17`, `openmp`, `sycl`, `image-processing`, `qt6`, `gpu-acceleration`, `polymorphism`, `cmake`

2. **Edit Description:**
   - Click ⚙️ next to "About"
   - Description: "High-performance image processing application demonstrating polymorphism, OpenMP CPU parallelization, and SYCL GPU acceleration"

3. **Add License** (Optional but recommended):
   ```bash
   # Create LICENSE file
   cat > LICENSE << 'EOF'
   MIT License

   Copyright (c) 2026 Rowan Houpa

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   EOF

   git add LICENSE
   git commit -m "Add MIT License"
   git push
   ```

### Share with Your Teacher

Once pushed, share this information:

**Repository Link:**
```
https://github.com/YourUsername/ImageFlow
```

**Key Files to Highlight:**
- `README.md` - Quick overview and getting started
- `report.tex` - Comprehensive technical documentation
- `core/include/Filter.hpp` - Polymorphism demonstration
- `core/src/filters/GrayscaleFilter.cpp` - OpenMP example
- `core/src/filters/GrayscaleFilterGPU.cpp` - SYCL example

## Making Updates

After making changes to your code:

```bash
# Check what changed
git status

# Add changes
git add .

# Commit with message
git commit -m "Description of changes"

# Push to GitHub
git push
```

## Common Issues

### Issue: "fatal: remote origin already exists"

```bash
# Remove existing remote
git remote remove origin

# Add new one
git remote add origin https://github.com/YourUsername/ImageFlow.git
```

### Issue: "Authentication failed"

- For HTTPS: Use Personal Access Token instead of password
- For SSH: Make sure SSH key is added to GitHub

### Issue: "! [rejected] main -> main (fetch first)"

Someone else made changes. Pull first:
```bash
git pull origin main --rebase
git push
```

### Issue: "Permission denied (publickey)"

SSH key not configured. Either:
1. Use HTTPS URL instead
2. Or add SSH key to GitHub (see Option 2 above)

## Repository Structure on GitHub

After pushing, your repository will show:

```
ImageFlow/
├── 📄 README.md                    ← Displays on homepage
├── 📄 report.tex                   ← Technical documentation
├── 📁 core/                        ← Core library
│   ├── include/                    ← Headers
│   └── src/                        ← Implementation
├── 📁 gui/                         ← Qt GUI
├── 📄 imageflow_cli.cpp            ← CLI
├── 📄 benchmark.cpp                ← Benchmarks
├── 🔧 CMakeLists.txt               ← Build config
├── 🚀 run_gui.sh                   ← Launcher scripts
└── 🚀 run_cli.sh
```

## GitHub Features to Use

### 1. Releases

Create a release for your teacher:

1. Go to "Releases" → "Create a new release"
2. Tag: `v1.0.0`
3. Title: `ImageFlow v1.0 - Initial Release`
4. Description: Copy from your README features section
5. Attach compiled `report.pdf` if you have it

### 2. README Badges

Your README already has badges! They'll display on GitHub:
- ![C++](https://img.shields.io/badge/C++-17-blue.svg)
- ![Qt](https://img.shields.io/badge/Qt-6.9.2-green.svg)
- ![OpenMP](https://img.shields.io/badge/OpenMP-Parallel-orange.svg)
- ![SYCL](https://img.shields.io/badge/SYCL-GPU-red.svg)

### 3. GitHub Pages (Optional)

Host your compiled PDF report:
1. Compile report: `./compile_report.sh`
2. Repository Settings → Pages
3. Enable Pages from main branch
4. Upload `report.pdf`

## Git Workflow Summary

```bash
# 1. Make changes to code
vim core/src/filters/NewFilter.cpp

# 2. Check what changed
git status
git diff

# 3. Stage changes
git add .

# 4. Commit
git commit -m "Add new filter: Edge Detection"

# 5. Push to GitHub
git push

# That's it! Changes are now on GitHub
```

## Help Commands

```bash
# View commit history
git log --oneline --graph

# See what's in last commit
git show

# See remote URL
git remote -v

# See current branch
git branch

# Undo last commit (keeps changes)
git reset --soft HEAD~1

# Discard all local changes
git reset --hard HEAD
```

## Support

If you encounter issues:
1. Check the error message carefully
2. Try the solutions in "Common Issues" above
3. GitHub has excellent documentation: https://docs.github.com
4. Git documentation: https://git-scm.com/doc
