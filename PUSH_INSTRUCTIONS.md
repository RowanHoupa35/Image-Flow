# Push to GitHub - Manual Instructions

Your ImageFlow repository is ready to push to:
**https://github.com/RowanHoupa35/Image-Flow**

## Quick Push (Copy-Paste This)

Open a terminal and run:

```bash
cd /home/rhoupa/ImageFlow
git push -u origin main
```

You will be prompted for:
1. **Username:** `RowanHoupa35`
2. **Password:** Use a **Personal Access Token** (see below)

## Get Personal Access Token

Since GitHub no longer accepts passwords, you need a Personal Access Token:

### Steps:

1. **Go to GitHub:**
   - Visit: https://github.com/settings/tokens

2. **Generate Token:**
   - Click "Generate new token" → "Generate new token (classic)"
   - Note: `ImageFlow Push Access`
   - Expiration: Choose "90 days" or "No expiration"
   - Select scopes: Check **"repo"** (Full control of private repositories)
   - Scroll down and click "Generate token"

3. **Copy the Token:**
   - **IMPORTANT:** Copy it immediately (you won't see it again!)
   - Example: `ghp_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`

4. **Use as Password:**
   - When `git push` asks for password, paste the token

### One-Time Setup (Saves Password)

```bash
# This will save your credentials after first successful push
git config --global credential.helper store

# Now push (you'll only need to enter credentials once)
git push -u origin main
```

## Alternative: Use SSH (More Secure)

If you prefer SSH authentication:

### 1. Generate SSH Key

```bash
ssh-keygen -t ed25519 -C "houparowan@gmail.com"
# Press Enter for all prompts (use defaults)
```

### 2. Copy Public Key

```bash
cat ~/.ssh/id_ed25519.pub
# Copy the entire output
```

### 3. Add to GitHub

1. Go to https://github.com/settings/keys
2. Click "New SSH key"
3. Title: `ImageFlow Development`
4. Key: Paste the copied public key
5. Click "Add SSH key"

### 4. Change Remote URL to SSH

```bash
git remote set-url origin git@github.com:RowanHoupa35/Image-Flow.git
git push -u origin main
# No password needed!
```

## Verify Push Succeeded

After pushing, visit:
**https://github.com/RowanHoupa35/Image-Flow**

You should see:
- ✅ 50 files
- ✅ README.md displayed on homepage
- ✅ All folders (core/, gui/, etc.)
- ✅ Latest commit: "Initial commit: ImageFlow..."

## Troubleshooting

### Error: "Authentication failed"
- Make sure you're using a Personal Access Token, not your GitHub password
- Token must have "repo" scope enabled

### Error: "Permission denied"
- For SSH: Make sure you added the public key to GitHub
- For HTTPS: Check token permissions

### Error: "Repository not found"
- Verify repository exists: https://github.com/RowanHoupa35/Image-Flow
- Check repository name matches exactly (case-sensitive)

## After Successful Push

1. **Verify on GitHub:**
   - Go to https://github.com/RowanHoupa35/Image-Flow
   - Check all files are there

2. **Add Topics:**
   - Click ⚙️ next to "About"
   - Add: `cpp`, `cpp17`, `openmp`, `sycl`, `image-processing`, `qt6`, `gpu-acceleration`, `polymorphism`

3. **Share with Teacher:**
   - Send link: https://github.com/RowanHoupa35/Image-Flow
   - Highlight: README.md and report.tex

## Status Check

Current repository status:

```bash
# View remote
git remote -v
# Should show:
# origin  https://github.com/RowanHoupa35/Image-Flow.git (fetch)
# origin  https://github.com/RowanHoupa35/Image-Flow.git (push)

# View commit
git log --oneline
# Should show:
# 6df5da6 Initial commit: ImageFlow - Image Processing with CPU/GPU Parallelization

# View files to be pushed
git ls-files | wc -l
# Should show: 50
```

## Need Help?

If you encounter issues:
1. Make sure you're in the ImageFlow directory: `cd /home/rhoupa/ImageFlow`
2. Check git status: `git status`
3. Try the SSH method (more reliable)
4. Ensure repository exists on GitHub
