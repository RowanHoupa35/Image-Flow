# VSCode + GitHub Integration Guide

## Quick Setup (Already Done!)

✅ VSCode workspace configured with Git integration
✅ Repository connected to GitHub: https://github.com/RowanHoupa35/Image-Flow
✅ Auto-save enabled for quick syncing
✅ Git commands integrated into VSCode

## Opening the Project in VSCode

```bash
cd /home/rhoupa/ImageFlow
code .
# or
/snap/bin/code .
```

## VSCode Git Features

### 1. Source Control Panel

**Access:** Click the Source Control icon (branch icon) in left sidebar, or press `Ctrl+Shift+G`

This panel shows:
- **Changes:** Files you've modified
- **Staged Changes:** Files ready to commit
- **Commit message box:** Enter your commit message here
- **Sync button:** Push/pull changes to/from GitHub

### 2. Making Changes and Syncing to GitHub

#### Method 1: Using Source Control Panel (Easiest)

1. **Edit files** in VSCode (they auto-save after 1 second)

2. **Open Source Control panel** (`Ctrl+Shift+G`)

3. **Stage changes:**
   - Click `+` next to individual files
   - Or click `+` next to "Changes" to stage all

4. **Write commit message** in the text box at top

5. **Commit:** Click the ✓ checkmark button

6. **Push to GitHub:** Click the "Sync Changes" button (circular arrows)

#### Method 2: Using Built-in Tasks (Quick)

1. **Press:** `Ctrl+Shift+P` (Command Palette)

2. **Type:** "Tasks: Run Task"

3. **Select:** "Git: Commit and Push"

4. **Enter commit message** when prompted

5. **Done!** Changes pushed to GitHub

#### Method 3: Using Terminal in VSCode

1. **Open terminal:** `` Ctrl+` `` (backtick) or View → Terminal

2. **Commands:**
   ```bash
   git add .
   git commit -m "Your commit message"
   git push
   ```

### 3. Quick Commit & Push Workflow

**Keyboard Shortcut Workflow:**

```
1. Edit files
2. Ctrl+Shift+G          → Open Source Control
3. Ctrl+Enter            → Commit staged changes
4. Click "Sync Changes"  → Push to GitHub
```

**Or use tasks:**

```
1. Edit files
2. Ctrl+Shift+P                  → Command Palette
3. Type "Run Task"
4. Select "Git: Commit and Push"
5. Enter message
6. Done!
```

## Git Status Indicators in VSCode

### File Colors in Explorer:
- **Green** (`U`) - New untracked file
- **Orange** (`M`) - Modified file
- **Red** (`D`) - Deleted file
- **Green** (`A`) - Added to staging
- **Gray** - Ignored by .gitignore

### Bottom Status Bar:
- **Branch icon + "main"** - Current branch
- **Sync icon (↑↓)** - Pending push/pull
- **Changes count** - Number of modified files

## Viewing Git History

### Using GitLens (Install Extension)

1. **Install:** Press `Ctrl+Shift+X`, search "GitLens", install
2. **View history:** Click GitLens icon in sidebar
3. **See commits:** Navigate through commit history
4. **Compare:** See what changed in each commit

### Using Git Graph (Install Extension)

1. **Install:** Search "Git Graph" in extensions
2. **Open Graph:** Click "Git Graph" in Source Control panel
3. **Visual history:** See commits as a graph

### Built-in Git History

1. Right-click any file
2. Select "Open Timeline"
3. See all commits affecting that file

## Common VSCode Git Operations

### Pull Latest Changes from GitHub

**Source Control Panel:**
- Click `...` (three dots) → Pull

**Command Palette:**
- `Ctrl+Shift+P` → "Git: Pull"

**Terminal:**
```bash
git pull origin main
```

### Create a Branch

**Source Control Panel:**
- Click branch name in bottom-left
- Select "Create new branch"
- Enter branch name

**Terminal:**
```bash
git checkout -b feature-name
```

### Switch Branch

- Click branch name in bottom-left
- Select branch from list

### Discard Changes

**Undo single file:**
- Source Control → Right-click file → Discard Changes

**Undo all changes:**
- Source Control → `...` → Discard All Changes

### View Diff (What Changed)

**Compare with last commit:**
- Click modified file in Source Control panel
- Side-by-side diff appears

**Compare with any commit:**
- Right-click file → "Open Timeline"
- Click commit to see diff

## Recommended VSCode Extensions

### Essential for Git:

1. **GitLens** (`eamodio.gitlens`)
   - Enhanced git blame, history, and comparisons
   - Install: `Ctrl+Shift+X` → search "GitLens"

2. **Git Graph** (`mhutchie.git-graph`)
   - Visual commit history
   - Shows branch relationships

3. **Git History** (`donjayamanne.githistory`)
   - View file history
   - Compare versions

### To Install Extensions:

1. Press `Ctrl+Shift+X`
2. Search extension name
3. Click "Install"
4. Reload VSCode if needed

Or install all at once via terminal:
```bash
code --install-extension eamodio.gitlens
code --install-extension mhutchie.git-graph
code --install-extension donjayamanne.githistory
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cmake-tools
```

## Auto-Sync Settings (Already Configured!)

Your `.vscode/settings.json` has:

```json
{
    "git.autofetch": true,              // Auto-fetch from GitHub
    "git.enableSmartCommit": true,      // Auto-stage when committing
    "git.confirmSync": false,           // No confirmation for sync
    "files.autoSave": "afterDelay",     // Auto-save files
    "files.autoSaveDelay": 1000         // Save after 1 second
}
```

This means:
- ✅ Files save automatically 1 second after editing
- ✅ VSCode fetches updates from GitHub automatically
- ✅ Commits can be made quickly without staging manually

## Typical Workflow

### Daily Work Routine:

```
1. Open VSCode: code /home/rhoupa/ImageFlow

2. Start working: Edit files (auto-saved)

3. Check changes: Ctrl+Shift+G (Source Control)

4. Commit:
   - Enter message in text box
   - Click ✓ checkmark

5. Push to GitHub: Click "Sync Changes" button

6. Continue working... repeat steps 2-5 as needed
```

### Before Showing to Teacher:

```
1. Make sure all changes are committed:
   - Check Source Control panel is empty

2. Final push:
   - Source Control → "Sync Changes"

3. Verify on GitHub:
   - Open: https://github.com/RowanHoupa35/Image-Flow
   - Check latest commit appears
```

## Keyboard Shortcuts Cheat Sheet

| Action | Shortcut |
|--------|----------|
| Open Source Control | `Ctrl+Shift+G` |
| Open Terminal | `` Ctrl+` `` |
| Command Palette | `Ctrl+Shift+P` |
| Stage All Changes | `Ctrl+K Ctrl+S` |
| Commit | `Ctrl+Enter` (in commit message box) |
| Save File | `Ctrl+S` |
| Save All | `Ctrl+K S` |
| Open File | `Ctrl+P` |
| Search in Project | `Ctrl+Shift+F` |

## Troubleshooting

### Issue: "Sync" button shows error

**Solution:**
```bash
# In VSCode terminal:
git pull origin main
git push origin main
```

### Issue: Changes not appearing on GitHub

**Check:**
1. Did you commit? (✓ button)
2. Did you push? (Sync button)
3. Check Source Control panel - should be empty after push

**Fix:**
```bash
git status          # See uncommitted changes
git add .           # Stage all
git commit -m "msg" # Commit
git push            # Push
```

### Issue: Merge conflicts

1. VSCode highlights conflicts in files
2. Choose "Accept Current" or "Accept Incoming"
3. Save file
4. Commit the merge

### Issue: Can't push (authentication)

Make sure you have credentials saved:
```bash
git config --global credential.helper store
git push  # Enter username and Personal Access Token once
```

## Advanced: Auto-Commit on Save (Optional)

If you want changes to auto-commit and push:

**Install extension:**
```bash
code --install-extension zeitnergmbh.auto-git
```

**Configure in settings:**
1. `Ctrl+,` → Settings
2. Search "Auto Git"
3. Enable "Auto Git: Enabled"
4. Set commit message template

**⚠️ Warning:** This commits every save, which creates many commits. Better to commit manually with meaningful messages.

## VSCode Git Panel Explained

```
Source Control Panel (Ctrl+Shift+G)
├── Commit message box      ← Type your message
├── ✓ Commit button        ← Click to commit
├── ... More Actions       ← Additional git commands
│   ├── Pull
│   ├── Push
│   ├── Fetch
│   └── ...
├── Changes                ← Modified files (red M)
│   ├── file1.cpp         ← Click to see diff
│   ├── file2.hpp         ← Hover: + to stage
│   └── ...
├── Staged Changes        ← Ready to commit (green A)
│   └── ...
└── Sync Changes button   ← Push to GitHub (bottom)
```

## Best Practices

### Good Commit Messages:

✅ **Good:**
- "Add sepia filter implementation"
- "Fix GPU memory leak in blur filter"
- "Update README with installation instructions"
- "Refactor FilterFactory for better performance"

❌ **Bad:**
- "update"
- "fix"
- "changes"
- "asdf"

### When to Commit:

- ✅ After implementing a feature
- ✅ After fixing a bug
- ✅ Before trying something risky
- ✅ At the end of work session
- ❌ Every single file save
- ❌ With broken/non-compiling code

### When to Push:

- After every commit (keep GitHub in sync)
- Before closing VSCode
- Before showing to teacher
- After completing a milestone

## Summary

**Your VSCode is now connected to GitHub!**

### To sync changes:
1. Edit files (auto-saved)
2. `Ctrl+Shift+G` (Source Control)
3. Write commit message
4. Click ✓ (Commit)
5. Click "Sync Changes" (Push)

### To verify:
- Visit: https://github.com/RowanHoupa35/Image-Flow
- See your latest commit

**That's it! Your changes are now on GitHub!** 🚀

## Quick Reference

```bash
# Open project in VSCode
cd /home/rhoupa/ImageFlow
code .

# VSCode Git workflow:
Ctrl+Shift+G     → Source Control
Type message     → Commit message
Ctrl+Enter       → Commit
Click "Sync"     → Push to GitHub

# Verify on GitHub:
https://github.com/RowanHoupa35/Image-Flow
```
