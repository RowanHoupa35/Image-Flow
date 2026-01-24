# VSCode + GitHub Quick Start

## ✅ Setup Complete!

Your VSCode is now connected to GitHub: https://github.com/RowanHoupa35/Image-Flow

## Open Project in VSCode

```bash
cd /home/rhoupa/ImageFlow
code .
```

Or double-click the folder in your file manager and select "Open with VSCode"

## Making Changes → GitHub (3 Steps!)

### Step 1: Edit Files
- Open any file in VSCode
- Make your changes
- Files auto-save after 1 second ✨

### Step 2: Commit
1. Press `Ctrl+Shift+G` (Source Control panel)
2. You'll see your changed files listed
3. Type a commit message in the box (e.g., "Fix bug in blur filter")
4. Press `Ctrl+Enter` or click the ✓ checkmark

### Step 3: Push to GitHub
- Click the **"Sync Changes"** button (with circular arrows)
- Your changes are now on GitHub!

## Visual Guide

```
┌─────────────────────────────────────────┐
│  VSCode Window                          │
├─────────────────────────────────────────┤
│  File  Edit  View  ...                 │
├──────┬──────────────────────────────────┤
│ [≡]  │  main.cpp                        │  ← Edit files here
│ [📁] │  class Foo {                     │
│ [🔍] │    void bar() { ... }            │  Auto-saves!
│ [⎇]  │  }                               │
│      │                                  │
├──────┴──────────────────────────────────┤
│ Bottom: main* ↑0↓0                     │  ← * means unsaved
└─────────────────────────────────────────┘

Press Ctrl+Shift+G:

┌─────────────────────────────────────────┐
│  SOURCE CONTROL                         │
├─────────────────────────────────────────┤
│  Message: Fix bug in blur filter____    │  ← Type here
│  [✓] Commit  [Sync Changes]             │  ← Click these
├─────────────────────────────────────────┤
│  Changes (3)                            │
│    M main.cpp                           │  ← Modified files
│    M Filter.hpp                         │
│    A NewFile.cpp                        │
└─────────────────────────────────────────┘
```

## Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| Open Source Control | `Ctrl+Shift+G` |
| Commit | `Ctrl+Enter` (in Source Control) |
| Open Terminal | `` Ctrl+` `` |
| Save File | `Ctrl+S` (already auto-saves!) |
| Command Menu | `Ctrl+Shift+P` |

## One-Command Commit & Push

**Even Faster Method:**

1. Press `Ctrl+Shift+P`
2. Type: `run task`
3. Select: **"Git: Commit and Push"**
4. Enter commit message
5. Done! Automatically commits and pushes to GitHub

## Status Indicators

### File Colors:
- **M** (Orange) = Modified
- **A** (Green) = Added new file
- **D** (Red) = Deleted
- **U** (Green) = Untracked (new file)

### Bottom Bar:
- **main** ← Your current branch
- **↑0↓0** ← Changes to push/pull
- Changes count

## Verify on GitHub

After pushing:
1. Open: https://github.com/RowanHoupa35/Image-Flow
2. Click "Commits"
3. See your latest commit with your message!

## Recommended Extensions

Open Extensions panel (`Ctrl+Shift+X`) and install:

1. **GitLens** - Enhanced Git features
   - Shows who changed what line
   - Visual commit history
   - Click "Install" when prompted

2. **Git Graph** - Visual commit tree
   - See all commits as a graph
   - Great for understanding history

VSCode will prompt you to install these when you open the project!

## Common Tasks

### See What Changed
- Click any file in Source Control panel
- Side-by-side diff appears showing old vs new

### Undo Changes
- Right-click file in Source Control
- Select "Discard Changes"

### Pull Latest from GitHub
- Source Control → `...` menu → Pull
- Or: `Ctrl+Shift+P` → "Git: Pull"

### Create New Branch
- Click "main" in bottom-left corner
- Select "Create new branch"
- Enter name

## Troubleshooting

### "Can't push" error
In terminal (`` Ctrl+` ``):
```bash
git pull origin main
git push origin main
```

### Files not syncing
Check Source Control panel:
- If files listed → need to commit
- If "Sync Changes" button shows → click it

### Want to see Git commands
- Terminal → `git log --oneline`
- Shows all your commits

## Pro Tips

### Auto-Save is ON
Your files save automatically 1 second after you stop typing. No need to press Ctrl+S!

### Check Before Closing
Before closing VSCode:
1. Open Source Control (`Ctrl+Shift+G`)
2. Should be empty (all committed)
3. If not, commit and sync

### Commit Often
Good practice:
- After completing a feature
- After fixing a bug
- Before trying something risky
- At end of work session

## Full Workflow Example

```
1. Open VSCode
   cd /home/rhoupa/ImageFlow && code .

2. Edit file
   Open core/src/filters/MyFilter.cpp
   Make changes (auto-saves!)

3. Commit
   Ctrl+Shift+G
   Type: "Add feature X to MyFilter"
   Ctrl+Enter

4. Push
   Click "Sync Changes"

5. Verify
   Open: https://github.com/RowanHoupa35/Image-Flow
   See your commit!

6. Continue working... repeat steps 2-5
```

## That's It!

**Your workflow is now:**
1. Edit → Auto-saves
2. `Ctrl+Shift+G` → Commit message → `Ctrl+Enter`
3. Click "Sync Changes"
4. Changes on GitHub! ✅

**See full guide:** `VSCODE_GITHUB_GUIDE.md`

**Your repository:** https://github.com/RowanHoupa35/Image-Flow
