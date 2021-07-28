// command, type, handler
SL_SS_TEST_POOL("write", SL_SS_TYPE_WRITE, sl_handle_ss_write)
SL_SS_TEST_POOL("read", SL_SS_TYPE_READ, sl_handle_ss_read)
SL_SS_TEST_POOL("delete", SL_SS_TYPE_DELETE, sl_handle_ss_delete)


* Welcome!

This is the configuration we've been building in the *Emacs From Scratch* series, now written as an Org Mode document.  This file generates [[file:init.el][init.el]] which can be loaded by Emacs at startup.

*NOTE:* If you run into any issues while using this configuration, please [[https://github.com/daviwil/emacs-from-scratch/issues/new][file an issue]] or send me an email at =david at systemcrafters.cc=.

The following variables are used to tweak some of the configuration pieces for use in the live streams so you might need to adjust them for your local machine if you try to use this configuration directly.

#+begin_src emacs-lisp

  ;; NOTE: init.el is now generated from Emacs.org.  Please edit that file
  ;;       in Emacs and init.el will be generated automatically!

  ;; You will most likely need to adjust this font size for your system!
  (defvar efs/default-font-size 125)
  (defvar efs/default-variable-font-size 100)
  (defvar kps/default-projectile-path "~/ext/WORKSPACE/")
  ;; Make frame transparency overridable
  (defvar efs/frame-transparency '(95 . 10))
  (defvar backup-directory "~/.backups")

  (fset 'yes-or-no-p 'y-or-n-p)

  ;;  (define-key global-map (kbd "C->") 'text-scale-increase)
  ;;  (define-key global-map (kbd "C-<") 'text-scale-decrease)
  (global-set-key [C-iso-lefttab] 'previous-buffer)
  (global-set-key [C-tab] 'next-buffer) 
  (global-set-key (kbd "<f5>") 'revert-buffer)
  (global-set-key (kbd "<f4>") 'goto-linez)
  (global-set-key [f7] 'comment-region)
  (global-set-key [f8] 'uncomment-region)
  (global-set-key (kbd "<f11>") 'highlight-symbol-at-point)
  (global-set-key [f9] 'bookmark-set)
	(global-set-key (kbd "C-c h o") 'occur)
	(global-set-key (kbd "C-c h r") 'rgrep)

  ;;(global-set-key [f12] 'bookmark-bmenu-list)
  ;;(global-set-key [f12] 'imenu-list-smart-toggle)

  (add-to-list 'auto-mode-alist '("\\Make.defs\\'" . makefile-mode))
  (add-to-list 'auto-mode-alist '("SConstruct" . python-mode))

#+end_src

* Startup Performance


#+begin_src emacs-lisp

  ;; The default is 800 kilobytes.  Measured in bytes.
  (setq gc-cons-threshold (* 50 1000 1000))

  (defun efs/display-startup-time ()
    (message "Emacs loaded in %s with %d garbage collections."
             (format "%.2f seconds"
                     (float-time
                      (time-subtract after-init-time before-init-time)))
             gcs-done))

  (add-hook 'emacs-startup-hook #'efs/display-startup-time)

  ;; Increase the amount of data which Emacs reads from the process. Again the emacs default is too 
  ;; low 4k considering that the some of the language server responses are in 800k - 3M range.
  (setq read-process-output-max (* 1024 1024)) ;; 1mb

  (setq global-mark-ring-max 5000         ; increase mark ring to contains 5000 entries
        mark-ring-max 5000                ; increase kill ring to contains 5000 entries
        mode-require-final-newline t      ; add a newline to end of file
        )

  ;; GROUP: Editing -> Killing
  (setq
   kill-ring-max 5000 ; increase kill-ring capacity
   kill-whole-line t  ; if NIL, kill whole line and move the next line up
   )

#+end_src
* Keep Folders Clean

We use the [[https://github.com/emacscollective/no-littering/blob/master/no-littering.el][no-littering]] package to keep folders where we edit files and the Emacs configuration folder clean!  It knows about a wide variety of variables for built in Emacs features as well as those from community packages so it can be much easier than finding and setting these variables yourself.

#+begin_src emacs-lisp
  ;; NOTE: If you want to move everything out of the ~/.emacs.d folder
  ;; reliably, set `user-emacs-directory` before loading no-littering!
  (setq user-emacs-directory "~/.cache/emacs")

  (use-package no-littering)

  ;; no-littering doesn't set this by default so we must place
  ;; auto save files in the same path as it uses for sessions
  (setq auto-save-file-name-transforms
        `((".*" ,(no-littering-expand-var-file-name "auto-save/") t)))

#+end_src

* Basic UI Configuration

This section configures basic UI settings that remove unneeded elements to make Emacs look a lot more minimal and modern.  If you're just getting started in Emacs, the menu bar might be helpful so you can remove the =(menu-bar-mode -1)= line if you'd like to still see that.

#+begin_src emacs-lisp

  (setq inhibit-startup-message t)

  (scroll-bar-mode -1)        ; Disable visible scrollbar
  (tool-bar-mode -1)          ; Disable the toolbar
  (tooltip-mode -1)           ; Disable tooltips
  (set-fringe-mode 10)        ; Give some breathing room

  (menu-bar-mode -1)            ; Disable the menu bar

  ;; Set up the visible bell
  (setq visible-bell t)

  (column-number-mode)
  (global-display-line-numbers-mode t)

  ;; Set frame transparency
  (set-frame-parameter (selected-frame) 'alpha efs/frame-transparency)
  (add-to-list 'default-frame-alist `(alpha . ,efs/frame-transparency))
  (set-frame-parameter (selected-frame) 'fullscreen 'maximized)
  (add-to-list 'default-frame-alist '(fullscreen . maximized))

  ;; Disable line numbers for some modes
  (dolist (mode '(org-mode-hook
                  term-mode-hook
                  shell-mode-hook
                  treemacs-mode-hook
                  eshell-mode-hook))
    (add-hook mode (lambda () (display-line-numbers-mode 0))))

#+end_src

** Font Configuration

I am using the [[https://github.com/tonsky/FiraCode][Fira Code]] and [[https://fonts.google.com/specimen/Cantarell][Cantarell]] fonts for this configuration which will more than likely need to be installed on your machine.  Both can usually be found in the various Linux distro package managers or downloaded from the links above.

#+begin_src emacs-lisp

(set-face-attribute 'default nil :font "Monaco" :height efs/default-font-size)

;; Set the fixed pitch face "Fira Code Retina"
(set-face-attribute 'fixed-pitch nil :font "Monaco" :height efs/default-font-size)

;; Set the variable pitch face Cantarell
(set-face-attribute 'variable-pitch nil :font "Monaco" :height efs/default-variable-font-size :weight 'regular)

#+end_src

* UI Configuration

** Command Log Mode

[[https://github.com/lewang/command-log-mode][command-log-mode]] is useful for displaying a panel showing each key binding you use in a panel on the right side of the frame.  Great for live streams and screencasts!

#+begin_src emacs-lisp

  (use-package command-log-mode
    :commands command-log-mode)

#+end_src

** Highlight line

#+begin_src emacs-lisp

  (global-hl-line-mode t)

#+end_src

** Color Theme
*** Doom(x)
[[https://github.com/hlissner/emacs-doom-themes][doom-themes]] is a great set of themes with a lot of variety and support for many different Emacs modes.  Taking a look at the [[https://github.com/hlissner/emacs-doom-themes/tree/screenshots][screenshots]] might help you decide which one you like best.  You can also run =M-x counsel-load-theme= to choose between them easily.

#+begin_src emacs-lisp

;;(use-package doom-themes
;;  :init (load-theme 'doom-dark+ t))

#+end_src

*** VS code

#+begin_src emacs-lisp

	;; (use-package solaire-mode
	;; 	:config
	;; 	(solaire-global-mode +1))

	;; ;;
	;; A more complex, more lazy-loaded config
	;; 	(use-package solaire-mode
	;; 		;; Ensure solaire-mode is running in all solaire-mode buffers
	;; 		:hook (change-major-mode . turn-on-solaire-mode)
	;; 		;; ...if you use auto-revert-mode, this prevents solaire-mode from turning
	;; 		;; itself off every time Emacs reverts the file
	;; 		:hook (after-revert . turn-on-solaire-mode)
	;; 		;; To enable solaire-mode unconditionally for certain modes:
	;; 		:hook (ediff-prepare-buffer . solaire-mode)
	;; 		;; Highlight the minibuffer when it is activated:
	;; 		:hook (minibuffer-setup . solaire-mode-in-minibuffer)
	;; 		:config
	;; 		;; The bright and dark background colors are automatically swapped the first 
	;; 		;; time solaire-mode is activated. Namely, the backgrounds of the `default` and
	;; 		;; `solaire-default-face` faces are swapped. This is done because the colors 
	;; 		;; are usually the wrong way around. If you don't want this, you can disable it:
	;; 		(setq solaire-mode-auto-swap-bg nil)

	;; (setq solaire-mode-remap-modeline nil)

	;; 		(solaire-global-mode +1))

	(use-package vscode-dark-plus-theme
		:config
		(load-theme 'vscode-dark-plus t))

#+end_src

** Better Modeline

[[https://github.com/seagle0128/doom-modeline][doom-modeline]] is a very attractive and rich (yet still minimal) mode line configuration for Emacs.  The default configuration is quite good but you can check out the [[https://github.com/seagle0128/doom-modeline#customize][configuration options]] for more things you can enable or disable.

*NOTE:* The first time you load your configuration on a new machine, you'll need to run `M-x all-the-icons-install-fonts` so that mode line icons display correctly.

#+begin_src emacs-lisp

	(use-package all-the-icons)

	(use-package doom-modeline
		:init (doom-modeline-mode 1)
		:custom ((doom-modeline-height 5))
		:config
		(setq doom-modeline-bar-width 4)

		;; Whether to use hud instead of default bar. It's only respected in GUI.
		;(defcustom doom-modeline-hud 1)

		;; The limit of the window width.
		;; If `window-width' is smaller than the limit, some information won't be displayed.
		(setq doom-modeline-window-width-limit fill-column)

		;; How to detect the project root.
		;; The default priority of detection is `ffip' > `projectile' > `project'.
		;; nil means to use `default-directory'.
		;; The project management packages have some issues on detecting project root.
		;; e.g. `projectile' doesn't handle symlink folders well, while `project' is unable
		;; to hanle sub-projects.
		;; You can specify one if you encounter the issue.
		(setq doom-modeline-project-detection 'project)

		;; Determines the style used by `doom-modeline-buffer-file-name'.
		;;
		;; Given ~/Projects/FOSS/emacs/lisp/comint.el
		;;   auto => emacs/lisp/comint.el (in a project) or comint.el
		;;   truncate-upto-project => ~/P/F/emacs/lisp/comint.el
		;;   truncate-from-project => ~/Projects/FOSS/emacs/l/comint.el
		;;   truncate-with-project => emacs/l/comint.el
		;;   truncate-except-project => ~/P/F/emacs/l/comint.el
		;;   truncate-upto-root => ~/P/F/e/lisp/comint.el
		;;   truncate-all => ~/P/F/e/l/comint.el
		;;   truncate-nil => ~/Projects/FOSS/emacs/lisp/comint.el
		;;   relative-from-project => emacs/lisp/comint.el
		;;   relative-to-project => lisp/comint.el
		;;   file-name => comint.el
		;;   buffer-name => comint.el<2> (uniquify buffer name)
		;;
		;; If you are experiencing the laggy issue, especially while editing remote files
		;; with tramp, please try `file-name' style.
		;; Please refer to https://github.com/bbatsov/projectile/issues/657.
		(setq doom-modeline-buffer-file-name-style 'auto)

		;; Whether display icons in the mode-line.
		;; While using the server mode in GUI, should set the value explicitly.
		(setq doom-modeline-icon (display-graphic-p))

		;; Whether display the icon for `major-mode'. It respects `doom-modeline-icon'.
		(setq doom-modeline-major-mode-icon t)

		;; Whether display the colorful icon for `major-mode'.
		;; It respects `all-the-icons-color-icons'.
		(setq doom-modeline-major-mode-color-icon t)

		;; Whether display the icon for the buffer state. It respects `doom-modeline-icon'.
		(setq doom-modeline-buffer-state-icon t)

		;; Whether display the modification icon for the buffer.
		;; It respects `doom-modeline-icon' and `doom-modeline-buffer-state-icon'.
		(setq doom-modeline-buffer-modification-icon t)

		;; Whether to use unicode as a fallback (instead of ASCII) when not using icons.
		(setq doom-modeline-unicode-fallback nil)

		;; Whether display the minor modes in the mode-line.
		(setq doom-modeline-minor-modes nil)

		;; If non-nil, a word count will be added to the selection-info modeline segment.
		(setq doom-modeline-enable-word-count nil)

		;; Major modes in which to display word count continuously.
		;; Also applies to any derived modes. Respects `doom-modeline-enable-word-count'.
		;; If it brings the sluggish issue, disable `doom-modeline-enable-word-count' or
		;; remove the modes from `doom-modeline-continuous-word-count-modes'.
		(setq doom-modeline-continuous-word-count-modes '(markdown-mode gfm-mode org-mode))

		;; Whether display the buffer encoding.
		(setq doom-modeline-buffer-encoding t)

		;; Whether display the indentation information.
		(setq doom-modeline-indent-info nil)

		;; If non-nil, only display one number for checker information if applicable.
		(setq doom-modeline-checker-simple-format t)

		;; The maximum number displayed for notifications.
		(setq doom-modeline-number-limit 99)

		;; The maximum displayed length of the branch name of version control.
		(setq doom-modeline-vcs-max-length 12)

		;; Whether display the workspace name. Non-nil to display in the mode-line.
		(setq doom-modeline-workspace-name t)

		;; Whether display the perspective name. Non-nil to display in the mode-line.
		(setq doom-modeline-persp-name t)

		;; If non nil the default perspective name is displayed in the mode-line.
		(setq doom-modeline-display-default-persp-name nil)

		;; If non nil the perspective name is displayed alongside a folder icon.
		(setq doom-modeline-persp-icon t)

		;; Whether display the `lsp' state. Non-nil to display in the mode-line.
		(setq doom-modeline-lsp t)

		;; Whether display the GitHub notifications. It requires `ghub' package.
		(setq doom-modeline-github nil)

		;; The interval of checking GitHub.
		(setq doom-modeline-github-interval (* 30 60))

		;; Whether display the modal state icon.
		;; Including `evil', `overwrite', `god', `ryo' and `xah-fly-keys', etc.
		(setq doom-modeline-modal-icon t)

		;; Whether display the gnus notifications.
		(setq doom-modeline-gnus t)

		;; Wheter gnus should automatically be updated and how often (set to 0 or smaller than 0 to disable)
		(setq doom-modeline-gnus-timer 2)

		;; Wheter groups should be excludede when gnus automatically being updated.
		(setq doom-modeline-gnus-excluded-groups '("dummy.group"))

		;; Whether display the IRC notifications. It requires `circe' or `erc' package.
		(setq doom-modeline-irc t)

		;; Function to stylize the irc buffer names.
		(setq doom-modeline-irc-stylize 'identity)

		;; Whether display the environment version.
		(setq doom-modeline-env-version t)
		;; Or for individual languages
		(setq doom-modeline-env-enable-python t)
		(setq doom-modeline-env-enable-ruby t)
		(setq doom-modeline-env-enable-perl t)
		(setq doom-modeline-env-enable-go t)
		(setq doom-modeline-env-enable-elixir t)
		(setq doom-modeline-env-enable-rust t)

		;; Change the executables to use for the language version string
		(setq doom-modeline-env-python-executable "python") ; or `python-shell-interpreter'
		(setq doom-modeline-env-ruby-executable "ruby")
		(setq doom-modeline-env-perl-executable "perl")
		(setq doom-modeline-env-go-executable "go")
		(setq doom-modeline-env-elixir-executable "iex")
		(setq doom-modeline-env-rust-executable "rustc")

		;; What to display as the version while a new one is being loaded
		(setq doom-modeline-env-load-string "...")

		;; Hooks that run before/after the modeline version string is updated
		(setq doom-modeline-before-update-env-hook nil)
		(setq doom-modeline-after-update-env-hook nil))

#+end_src

** Which Key

[[https://github.com/justbur/emacs-which-key][which-key]] is a useful UI panel that appears when you start pressing any key binding in Emacs to offer you all possible completions for the prefix.  For example, if you press =C-c= (hold control and press the letter =c=), a panel will appear at the bottom of the frame displaying all of the bindings under that prefix and which command they run.  This is very useful for learning the possible key bindings in the mode of your current buffer.

#+begin_src emacs-lisp

  (use-package which-key
    :defer 0
    :diminish which-key-mode
    :config
    (which-key-mode)
    (setq which-key-idle-delay 1))

#+end_src

** Ivy and Counsel

[[https://oremacs.com/swiper/][Ivy]] is an excellent completion framework for Emacs.  It provides a minimal yet powerful selection menu that appears when you open files, switch buffers, and for many other tasks in Emacs.  Counsel is a customized set of commands to replace `find-file` with `counsel-find-file`, etc which provide useful commands for each of the default completion commands.

[[https://github.com/Yevgnen/ivy-rich][ivy-rich]] adds extra columns to a few of the Counsel commands to provide more information about each item.

#+begin_src emacs-lisp

  (use-package counsel
    :bind (("C-M-j" . 'counsel-switch-buffer)
           :map minibuffer-local-map
           ("C-r" . 'counsel-minibuffer-history))
    :custom
    (counsel-linux-app-format-function #'counsel-linux-app-format-function-name-only)
    :config
    (counsel-mode 1))

#+end_src

** Smart move to beginning of line
- Move point back to indentation of beginning of line. Move point to the first non-whitespace character on this line. If point is already there, move to the beginning of the line. Effectively toggle between the first non-whitespace character and the beginning of the line. If ARG is not nil or 1, move forward ARG - 1 lines first.  If point reaches the beginning or end of the buffer, stop there.
  #+BEGIN_SRC emacs-lisp

    (defun smarter-move-beginning-of-line (arg)
      (interactive "^p")
      (setq arg (or arg 1))

      ;; Move lines first
      (when (/= arg 1)
        (let ((line-move-visual nil))
          (forward-line (1- arg))))

      (let ((orig-point (point)))
        (back-to-indentation)
        (when (= orig-point (point))
          (move-beginning-of-line 1))))

    ;; remap C-a to `smarter-move-beginning-of-line'
    (global-set-key [remap move-beginning-of-line]
                    'smarter-move-beginning-of-line)

  #+END_SRC

** Undo-tree

#+BEGIN_SRC emacs-lisp

  (use-package undo-tree
    :init
    (global-undo-tree-mode))

#+END_SRC
** Highlight
*** Highlight-symbol

#+BEGIN_SRC emacs-lisp

	(use-package highlight-symbol
		:config
		(set-face-attribute 'highlight-symbol-face nil
												:background "default"
												:foreground "#FA009A")
		(setq highlight-symbol-idle-delay 0
					highlight-symbol-on-navigation-p t)
		(add-hook 'prog-mode-hook #'highlight-symbol-mode)
		(add-hook 'prog-mode-hook #'highlight-symbol-nav-mode)
		(global-set-key [(control shift mouse-1)]
										(lambda (event)
											(interactive "e")
											(goto-char (posn-point (event-start event)))
											(highlight-symbol-at-point)))
	;;  (global-set-key [(control f3)] 'highlight-symbol)
		(global-set-key [(meta f3)] 'highlight-symbol-query-replace)
		(global-set-key (kbd "M-n") 'highlight-symbol-next)
		(global-set-key (kbd "M-p") 'highlight-symbol-prev))

#+END_SRC

*** highlight-numbers
#+BEGIN_SRC emacs-lisp

(use-package highlight-numbers
  :config
  (add-hook 'prog-mode-hook 'highlight-numbers-mode))

#+END_SRC

** Helm

*** Helm

#+begin_src emacs-lisp

		(use-package helm
			:diminish
			:bind-keymap
			("C-c h" . helm-command-map)
			:bind (("M-x" . helm-M-x)
						 ("M-y" . helm-show-kill-ring)
						 ("C-x b" . helm-mini)
						 ("C-x C-f" . helm-find-files)
						 ("C-h SPC" . helm-all-mark-rings)
						 :map helm-command-map
						 ("x" . helm-register)
						 ("M-:" . helm-eval-expression-with-eldoc)
						 ("o" . helm-occur)
						 :map helm-map
						 ("C-i" . helm-execute-persistent-action)
						 ("TAB" . helm-execute-persistent-action) ; make TAB work in terminal
						 ("C-z" . helm-select-action) ; list actions using C-z
						 )
			:config
			(setq helm-idle-delay 0.1)
			(setq helm-input-idle-delay 0.1)
			;; The default "C-x c" is quite close to "C-x C-c", which quits Emacs.
			;; Changed to "C-c h". Note: We must set "C-c h" globally, because we
			;; cannot change `helm-command-prefix-key' once `helm-config' is loaded.

			(setq helm-split-window-in-side-p           -1 ; open helm buffer inside current window, not occupy whole other window
						helm-move-to-line-cycle-in-source   t ; move to end or beginning of source when reaching top or bottom of source.
						helm-ff-search-library-in-sexp        t ; search for library in `require' and `declare-function' sexp.
						helm-scroll-amount                    8 ; scroll 8 lines other window using M-<next>/M-<prior>
						helm-ff-file-name-history-use-recentf t
						helm-echo-input-in-header-line t)
			(setq helm-autoresize-max-height 25)
			(setq helm-autoresize-min-height 25)
			;;(setq helm-default-display-buffer-functions '(display-buffer-in-atom-window))
			(setq helm-locate-fuzzy-match t)
			(helm-autoresize-mode 1)
			(helm-mode 1))

		(when (executable-find "ack-grep")
			(setq helm-grep-default-command "ack-grep -Hn --no-group --no-color %e %p %f"
						helm-grep-default-recurse-command "ack-grep -H --no-group --no-color %e %p %f"))

		(setq helm-buffers-fuzzy-matching t
					helm-recentf-fuzzy-match    t
					helm-imenu-fuzzy-match t)

		(setq helm-M-x-fuzzy-match t) ;; optional fuzzy matching for helm-M-x

		(when (executable-find "curl")
			(setq helm-google-suggest-use-curl-p t))

#+end_src

*** helm-ag

#+begin_src emacs-lisp
  (use-package helm-ag
    :after helm
    :config
    (custom-set-variables
     '(helm-ag-base-command "ag --nocolor --nogroup --ignore-case")
     '(helm-ag-command-option "--all-text")
     '(helm-ag-insert-at-point 'symbol)
     '(helm-ag-ignore-buffer-patterns '("\\.txt\\'" "\\.mkd\\'"))))

#+end_src

**** Linux

#+begin_src sh :tangle no

apt install silversearcher-ag

#+end_src

*** helm-swiper

#+begin_src emacs-lisp

  (use-package swiper-helm
    :after helm
    :bind (("C-s" . swiper-helm))
    )

#+end_src
** Helpful Help Commands

[[https://github.com/Wilfred/helpful][Helpful]] adds a lot of very helpful (get it?) information to Emacs' =describe-= command buffers.  For example, if you use =describe-function=, you will not only get the documentation about the function, you will also see the source code of the function and where it gets used in other places in the Emacs configuration.  It is very useful for figuring out how things work in Emacs.

#+begin_src emacs-lisp

  (use-package helpful
    :commands (helpful-callable helpful-variable helpful-command helpful-key)
    :custom
    (counsel-describe-function-function #'helpful-callable)
    (counsel-describe-variable-function #'helpful-variable)
    :bind
    ([remap describe-function] . counsel-describe-function)
    ([remap describe-command] . helpful-command)
    ([remap describe-variable] . counsel-describe-variable)
    ([remap describe-key] . helpful-key))

#+end_src

** Encoding

#+BEGIN_SRC emacs-lisp

  (set-terminal-coding-system 'utf-8)
  (set-keyboard-coding-system 'utf-8)
  (set-language-environment "UTF-8")
  (prefer-coding-system 'utf-8)

  (set-language-environment "Korean")
  (prefer-coding-system 'utf-8)

#+END_SRC

** Adaptive-Wrap
- Show a nice new line

  #+BEGIN_SRC emacs-lisp

    (use-package adaptive-wrap
      :ensure t
      :config
      (when (fboundp 'adaptive-wrap-prefix-mode)
        (defun my-activate-adaptive-wrap-prefix-mode ()
          "Toggle `visual-line-mode' and `adaptive-wrap-prefix-mode' simultaneously."
          (adaptive-wrap-prefix-mode (if visual-line-mode 1 -1)))
        (add-hook 'visual-line-mode-hook 'my-activate-adaptive-wrap-prefix-mode)))

  #+END_SRC

** Tabbar (X)

#+begin_src emacs-lisp

	(use-package tabbar
		:disabled
		:config
		(tabbar-mode t))

#+end_src

** Markdown
*** markdown
#+BEGIN_SRC emacs-lisp

  (use-package markdown-mode
    :commands (markdown-mode gfm-mode)
    :mode (("README\\.md\\'" . gfm-mode)
           ("\\.md\\'" . markdown-mode)
           ("\\.markdown\\'" . markdown-mode))
    :init (setq markdown-command "multimarkdown"))

#+END_SRC
*** Flymd
- On the fly markdown preview
**** Link
- [[https://github.com/mola-T/flymd][Homepage]]
  #+BEGIN_SRC emacs-lisp

    (use-package flymd)

  #+END_SRC

** revert
#+BEGIN_SRC emacs-lisp

 (setq save-interprogram-paste-before-kill t)

;; update any change made on file to the current buffer
(global-auto-revert-mode)
(setq auto-revert-verbose nil)

#+END_SRC

** whitespace
#+BEGIN_SRC emacs-lisp

  (add-hook 'prog-mode-hook (lambda () (interactive) (setq show-trailing-whitespace 1)))
  (define-key prog-mode-map (kbd "C-c w") 'whitespace-mode)
  ;;  (global-set-key (kbd "C-c w") 'whitespace-mode)

#+END_SRC

** savespace
#+begin_src emacs-lisp

  (use-package saveplace
    :config
    (setq-default save-place t)
    (toggle-save-place-globally 1))

#+end_src

** Beacon mode
- flashes the cursor's line when you scroll

#+BEGIN_SRC emacs-lisp
  (use-package beacon
    :config
    (beacon-mode 1)
    ;; this color looks good for the zenburn theme but not for the one
    ;; I'm using for the videos
    ;; (setq beacon-color "#666600")
    )
#+END_SRC
* Development

** Languages

*** IDE Features with lsp-mode

**** lsp-mode

We use the excellent [[https://emacs-lsp.github.io/lsp-mode/][lsp-mode]] to enable IDE-like functionality for many different programming languages via "language servers" that speak the [[https://microsoft.github.io/language-server-protocol/][Language Server Protocol]].  Before trying to set up =lsp-mode= for a particular language, check out the [[https://emacs-lsp.github.io/lsp-mode/page/languages/][documentation for your language]] so that you can learn which language servers are available and how to install them.

The =lsp-keymap-prefix= setting enables you to define a prefix for where =lsp-mode='s default keybindings will be added.  I *highly recommend* using the prefix to find out what you can do with =lsp-mode= in a buffer.

The =which-key= integration adds helpful descriptions of the various keys so you should be able to learn a lot just by pressing =C-c l= in a =lsp-mode= buffer and trying different things that you find there.

#+begin_src emacs-lisp

	(defun efs/lsp-mode-setup ()
		(setq lsp-headerline-breadcrumb-segments '(path-up-to-project file symbols))
		(lsp-headerline-breadcrumb-mode))

	(use-package lsp-mode
		:commands (lsp lsp-deferred)
		:hook (lsp-mode . efs/lsp-mode-setup)
		:init
		(setq lsp-keymap-prefix "C-c l")  ;; Or 'C-l', 's-l'
		:config
		(lsp-enable-which-key-integration t))

	(add-hook 'c-mode-hook 'lsp)
	(add-hook 'c++-mode-hook 'lsp)

#+end_src

**** lsp-ui

[[https://emacs-lsp.github.io/lsp-ui/][lsp-ui]] is a set of UI enhancements built on top of =lsp-mode= which make Emacs feel even more like an IDE.  Check out the screenshots on the =lsp-ui= homepage (linked at the beginning of this paragraph) to see examples of what it can do.

#+begin_src emacs-lisp

  (use-package lsp-ui
    :hook (lsp-mode . lsp-ui-mode)
    :custom
    (lsp-ui-doc-position 'bottom))

#+end_src

**** lsp-treemacs

[[https://github.com/emacs-lsp/lsp-treemacs][lsp-treemacs]] provides nice tree views for different aspects of your code like symbols in a file, references of a symbol, or diagnostic messages (errors and warnings) that are found in your code.

Try these commands with =M-x=:

- =lsp-treemacs-symbols= - Show a tree view of the symbols in the current file
- =lsp-treemacs-references= - Show a tree view for the references of the symbol under the cursor
- =lsp-treemacs-error-list= - Show a tree view for the diagnostic messages in the project

This package is built on the [[https://github.com/Alexander-Miller/treemacs][treemacs]] package which might be of some interest to you if you like to have a file browser at the left side of your screen in your editor.

#+begin_src emacs-lisp

	(use-package lsp-treemacs
		:after lsp
		:bind ("C-'" . lsp-treemacs-symbols)
		:config
		(lsp-treemacs-sync-mode 1))
;;		(add-hook 'prog-mode-hook 'lsp-treemacs-symbols))
(define-key prog-mode-map (kbd "C-'") 'lsp-treemacs-symbols)

#+end_src

**** lsp-ivy (X)

[[https://github.com/emacs-lsp/lsp-ivy][lsp-ivy]] integrates Ivy with =lsp-mode= to make it easy to search for things by name in your code.  When you run these commands, a prompt will appear in the minibuffer allowing you to type part of the name of a symbol in your code.  Results will be populated in the minibuffer so that you can find what you're looking for and jump to that location in the code upon selecting the result.

Try these commands with =M-x=:

- =lsp-ivy-workspace-symbol= - Search for a symbol name in the current project workspace
- =lsp-ivy-global-workspace-symbol= - Search for a symbol name in all active project workspaces

#+begin_src emacs-lisp

	(use-package lsp-ivy
		:disabled
		:after lsp)

#+end_src

*** Python

We use =lsp-mode= and =dap-mode= to provide a more complete development environment for Python in Emacs.  Check out [[https://emacs-lsp.github.io/lsp-mode/page/lsp-pyls/][the =pyls= configuration]] in the =lsp-mode= documentation for more details.

Make sure you have the =pyls= language server installed before trying =lsp-mode=!

#+begin_src sh :tangle no

pip install --user "python-language-server[all]"

#+end_src

There are a number of other language servers for Python so if you find that =pyls= doesn't work for you, consult the =lsp-mode= [[https://emacs-lsp.github.io/lsp-mode/page/languages/][language configuration documentation]] to try the others!

#+begin_src emacs-lisp

  (use-package python-mode
    :ensure t
    :hook (python-mode . lsp-deferred)
    :custom
    ;; NOTE: Set these if Python 3 is called "python3" on your system!
    ;; (python-shell-interpreter "python3")
    ;; (dap-python-executable "python3")
    (dap-python-debugger 'debugpy)
    :config
    (require 'dap-python))


#+end_src

You can use the pyvenv package to use =virtualenv= environments in Emacs.  The =pyvenv-activate= command should configure Emacs to cause =lsp-mode= and =dap-mode= to use the virtual environment when they are loaded, just select the path to your virtual environment before loading your project.

#+begin_src emacs-lisp

  (use-package pyvenv
    :after python-mode
    :config
    (pyvenv-mode 1))

#+end_src

** Style

#+begin_src emacs-lisp

  (add-hook 'prog-mode-hook (lambda () (interactive) (setq show-trailing-whitespace 1)))
  (delete-selection-mode)

  ;; set appearance of a tab that is represented by 4 spaces
  (setq c-default-style "linux") ; set style to "linux"
  ;;      c-basic-offset 4)
  (setq-default c-basic-offset 2
                tab-width 2
                indent-tabs-mode t)

#+end_src

** Company Mode

[[http://company-mode.github.io/][Company Mode]] provides a nicer in-buffer completion interface than =completion-at-point= which is more reminiscent of what you would expect from an IDE.  We add a simple configuration to make the keybindings a little more useful (=TAB= now completes the selection and initiates completion at the current location if needed).

We also use [[https://github.com/sebastiencs/company-box][company-box]] to further enhance the look of the completions with icons and better overall presentation.

#+begin_src emacs-lisp

	(use-package company
		:after lsp-mode
		:hook (lsp-mode . company-mode)
		:bind (:map company-active-map
					 ("<tab>" . company-complete-selection))
					(:map lsp-mode-map
					 ("<tab>" . company-indent-or-complete-common))
		:custom
		(company-minimum-prefix-length 1)
		(company-idle-delay 0.0))

	;; (use-package company-box
	;;   :hook (company-mode . company-box-mode))

#+end_src

** Projectile

[[https://projectile.mx/][Projectile]] is a project management library for Emacs which makes it a lot easier to navigate around code projects for various languages.  Many packages integrate with Projectile so it's a good idea to have it installed even if you don't use its commands directly.

#+begin_src emacs-lisp

  (use-package projectile
    :diminish projectile-mode
    :config (projectile-mode)
    :custom ((projectile-completion-system 'helm)
             (projectile-enable-caching t)
             (projectile-indexing-method 'alien))
    :bind-keymap
    ("C-c p" . projectile-command-map)
    :init
    ;; NOTE: Set this to the folder where you keep your Git repos!
    (when (file-directory-p "~/ext/WORKSPACE/")
      (setq projectile-project-search-path '("~/ext/WORKSPACE/")))
    (setq projectile-switch-project-action #'projectile-dired))

  (use-package helm-projectile
    :ensure t
    :config
    (helm-projectile-on)
    (setq projectile-switch-project-action 'helm-projectile))

#+end_src

** Magit

[[https://magit.vc/][Magit]] is the best Git interface I've ever used.  Common Git operations are easy to execute quickly using Magit's command panel system.

#+begin_src emacs-lisp

  (use-package magit
    :commands magit-status
    :custom
    (magit-display-buffer-function #'magit-display-buffer-same-window-except-diff-v1))

  ;; NOTE: Make sure to configure a GitHub token before using this package!
  ;; - https://magit.vc/manual/forge/Token-Creation.html#Token-Creation
  ;; - https://magit.vc/manual/ghub/Getting-Started.html#Getting-Started
  (use-package forge
    :after magit)

#+end_src


** Yasnippet
#+begin_src emacs-lisp

  (use-package yasnippet
    :config
    (yas-global-mode 1))

#+end_src

*** snipepets

#+begin_src emacs-lisp

  (use-package yasnippet-snippets
    :after yasnippet)

#+end_src
** Rainbow Delimiters

[[https://github.com/Fanael/rainbow-delimiters][rainbow-delimiters]] is useful in programming modes because it colorizes nested parentheses and brackets according to their nesting depth.  This makes it a lot easier to visually match parentheses in Emacs Lisp code without having to count them yourself.

#+begin_src emacs-lisp

(use-package rainbow-delimiters
  :hook (prog-mode . rainbow-delimiters-mode))

#+end_src
** Compile

#+BEGIN_SRC emacs-lisp 

		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		;; GROUP: Programming -> Tools -> Compilation ;;
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		;; Compilation from Emacs
		(defun prelude-colorize-compilation-buffer ()
			"Colorize a compilation mode buffer."
			(interactive)
			;; we don't want to mess with child modes such as grep-mode, ack, ag, etc
			(when (eq major-mode 'compilation-mode)
				(let ((inhibit-read-only t))
					(ansi-color-apply-on-region (point-min) (point-max)))))

		;; setup compilation-mode used by `compile' command
		;; 	(defun kps/my_compile_path ()
		;; (interactive)
		;; 		(defvar kps/cur_path (projectile-project-root))
		;; ;;		(setq compile-command kps/cur_path)
		;; 		(compile 'kps/cur_path)
		;; 		)

		(defun kps/compile-next-makefile ()
			(interactive)
			(let* ((compile-command (concat (projectile-project-root) "os/dbuild.sh")))
				(compile compile-command)))

		(use-package compile
			:config
			(setq compilation-ask-about-save nil          ; Just save before compiling
						compilation-always-kill t               ; Just kill old compile processes before starting the new one
						compilation-scroll-output 'first-error) ; Automatically scroll to first
			;;(define-key prog-mode-map (kbd "<C-f5>") 'kps/compile-next-makefile)
			)

	(define-key prog-mode-map 
		(kbd "<C-f5>") 
		(lambda ()
			(interactive)
			;;(setq-local compilation-read-command nil)
			(setq-local compile-command (concat (projectile-project-root) "os/dbuild.sh"))
			(call-interactively 'compile)))

#+END_SRC

** Makefile

#+BEGIN_SRC emacs-lisp
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;;
  ;; GROUP: Programming -> Tools -> Makefile
  ;; takenn from prelude-c.el:48: https://github.com/bbatsov/prelude/blob/master/modules/prelude-c.el
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (defun prelude-makefile-mode-defaults ()
    (whitespace-toggle-options '(tabs))
    (setq indent-tabs-mode t))

  (setq prelude-makefile-mode-hook 'prelude-makefile-mode-defaults)

  (add-hook 'makefile-mode-hook (lambda ()
                                  (run-hooks 'prelude-makefile-mode-hook)))

  ;; GROUP: Programming -> Tools -> Ediff
  (setq ediff-diff-options "-w"
        ediff-split-window-function 'split-window-horizontally
        ediff-window-setup-function 'ediff-setup-windows-plain)

#+END_SRC

** GDB

#+BEGIN_SRC emacs-lisp

  (setq gdb-many-windows t      ; use gdb-many-windows by default
        gdb-show-main t)        ; Non-nil means display source file containing the main routine at startup

  (setq gud-chdir-before-run nil)

#+END_SRC


** iedit

#+begin_src emacs-lisp

  (use-package iedit)

  ;; if you're windened, narrow to the region, if you're narrowed, widen
  ;; bound to C-x n
  (defun narrow-or-widen-dwim (p)
    "If the buffer is narrowed, it widens. Otherwise, it narrows intelligently.
    Intelligently means: region, org-src-block, org-subtree, or defun,
    whichever applies first.
    Narrowing to org-src-block actually calls `org-edit-src-code'.

    With prefix P, don't widen, just narrow even if buffer is already
    narrowed."
    (interactive "P")
    (declare (interactive-only))
    (cond ((and (buffer-narrowed-p) (not p)) (widen))
          ((region-active-p)
           (narrow-to-region (region-beginning) (region-end)))
          ((derived-mode-p 'org-mode)
           ;; `org-edit-src-code' is not a real narrowing command.
           ;; Remove this first conditional if you don't want it.
           (cond ((ignore-errors (org-edit-src-code))
                  (delete-other-windows))
                 ((org-at-block-p)
                  (org-narrow-to-block))
                 (t (org-narrow-to-subtree))))
          (t (narrow-to-defun))))

  ;; (define-key endless/toggle-map "n" #'narrow-or-widen-dwim)
  ;; This line actually replaces Emacs' entire narrowing keymap, that's
  ;; how much I like this command. Only copy it if that's what you want.
  (define-key ctl-x-map "n" #'narrow-or-widen-dwim)

  (defun iedit-dwim (arg)
    "Starts iedit but uses \\[narrow-to-defun] to limit its scope."
    (interactive "P")
    (if arg
        (iedit-mode)
      (save-excursion
        (save-restriction
          (widen)
          ;; this function determines the scope of `iedit-start'.
          (if iedit-mode
              (iedit-done)
            ;; `current-word' can of course be replaced by other
            ;; functions.
            (narrow-to-defun)
            (iedit-start (current-word) (point-min) (point-max)))))))

#+end_src

** Kconfig mode

#+BEGIN_SRC emacs-lisp
  ;;; kconfig.el - a major mode for editing linux kernel config (Kconfig) files
  ;; Copyright © 2014 Yu Peng
  ;; Copyright © 2014 Michal Sojka

  (defvar kconfig-mode-font-lock-keywords
    '(("^[\t, ]*\\_<bool\\_>" . font-lock-type-face)
      ("^[\t, ]*\\_<int\\_>" . font-lock-type-face)
      ("^[\t, ]*\\_<boolean\\_>" . font-lock-type-face)
      ("^[\t, ]*\\_<tristate\\_>" . font-lock-type-face)
      ("^[\t, ]*\\_<depends on\\_>" . font-lock-variable-name-face)
      ("^[\t, ]*\\_<select\\_>" . font-lock-variable-name-face)
      ("^[\t, ]*\\_<help\\_>" . font-lock-variable-name-face)
      ("^[\t, ]*\\_<---help---\\_>" . font-lock-variable-name-face)
      ("^[\t, ]*\\_<default\\_>" . font-lock-variable-name-face)
      ("^[\t, ]*\\_<range\\_>" . font-lock-variable-name-face)
      ("^\\_<config\\_>" . font-lock-constant-face)
      ("^\\_<comment\\_>" . font-lock-constant-face)
      ("^\\_<menu\\_>" . font-lock-constant-face)
      ("^\\_<endmenu\\_>" . font-lock-constant-face)
      ("^\\_<if\\_>" . font-lock-constant-face)
      ("^\\_<endif\\_>" . font-lock-constant-face)
      ("^\\_<menuconfig\\_>" . font-lock-constant-face)
      ("^\\_<source\\_>" . font-lock-keyword-face)
      ("\#.*" . font-lock-comment-face)
      ("\".*\"$" . font-lock-string-face)))

  (defvar kconfig-headings
    '("bool" "int" "boolean" "tristate" "depends on" "select"
      "help" "---help---" "default" "range" "config" "comment"
      "menu" "endmenu" "if" "endif" "menuconfig" "source"))

  (defun kconfig-outline-level ()
    (looking-at "[\t ]*")
    (let ((prefix (match-string 0))
          (result 0))
      (dotimes (i (length prefix) result)
        (setq result (+ result
                        (if (equal (elt prefix i) ?\s)
                            1 tab-width))))))

  (define-derived-mode kconfig-mode text-mode
    "kconfig"
    (set (make-local-variable 'font-lock-defaults)
         '(kconfig-mode-font-lock-keywords t))
    (set (make-local-variable 'outline-regexp)
         (concat "^[\t ]*" (regexp-opt kconfig-headings)))
    (set (make-local-variable 'outline-level)
         'kconfig-outline-level))

  (add-to-list 'auto-mode-alist '("Kconfig" . kconfig-mode))
#+END_SRC

* Edit

** avy
- avy is a GNU Emacs package for jumping to visible text using a char-based decision tree. See also ace-jump-mode and vim-easymotion avy uses the same idea.
- I don't need to use arrow key anymore. I can move to where I want to go by this application.

#+BEGIN_SRC emacs-lisp
  (use-package avy
    :bind ("C-c m" . avy-goto-char))
#+END_SRC

** Ace-windows

#+BEGIN_SRC emacs-lisp

  (use-package ace-window
    :init
    (progn
      (global-set-key [remap other-window] 'ace-window)
      (custom-set-faces
       '(aw-leading-char-face
         ((t (:inherit ace-jump-face-foreground :height 3.0)))))))

  (defun pk/swap-windows ()
    "Swap windows"
    (interactive)
    (ace-swap-window)
    (aw-flip-window))

  (defun pk/my-windows ()
    "Go to ilist"
    (interactive)
    (print (buffer-name))
    (ace-window 1))

  ;;(buffer-name)
  (global-set-key (kbd "C-x m") 'pk/swap-windows)

#+END_SRC


** Smartparens
- [[https://github.com/Fuco1/smartparens][Homepage]]
*** Usage
#+BEGIN_SRC emacs-lisp

  (use-package smartparens
    :config
    (setq sp-base-key-bindings 'paredit)
    (setq sp-autoskip-closing-pair 'always)
    (setq sp-hybrid-kill-entire-symbol nil)
    (sp-use-paredit-bindings)
    (show-smartparens-global-mode +1)
    (smartparens-global-mode 1)
    ;; when you press RET, the curly braces automatically
    ;; add another newline
    (sp-with-modes '(c-mode c++-mode)
           (sp-local-pair "{" nil :post-handlers '(("||\n[i]" "RET")))
           (sp-local-pair "/*" "*/" :post-handlers '((" | " "SPC")
                                 ("* ||\n[i]" "RET")))))

#+END_SRC

** Multiple-Cursor

#+BEGIN_SRC emacs-lisp

  (use-package multiple-cursors
    :config
    (when (fboundp 'mc/edit-lines)
      (global-set-key (kbd "C-S-c C-S-c") 'mc/edit-lines)
      ;;(global-set-key (kbd "C->") 'mc/mark-next-like-this)
      (global-set-key (kbd "C->") 'mc/mark-next-like-this-word)
      (global-set-key (kbd "C-<") 'mc/mark-previous-like-this)
      (global-set-key (kbd "C-c C-<") 'mc/mark-all-like-this)
      )
    (define-key global-map (kbd "H-%") 'mqr-query-replace)
    (define-key global-map (kbd "H-#") 'mqr-query-replace-regexp))

#+END_SRC

** Macro

#+BEGIN_SRC emacs-lisp

  (global-set-key (kbd "<C-f3>") 'kmacro-start-macro-or-insert-counter)
  (global-set-key (kbd "<f3>") 'kmacro-end-and-call-macro)

#+END_SRC

** Hungry delete mode
- Delete all the whitespace when you hit backspace or delete

#+BEGIN_SRC emacs-lisp
  
(use-package hungry-delete
    :ensure t
    :config
    (global-hungry-delete-mode))

#+END_SRC

** Expand Region

#+BEGIN_SRC emacs-lisp

  (use-package expand-region
    :config
    (global-set-key (kbd "C-=") 'er/expand-region);
    (global-set-key (kbd "C-c b") 'er/contract-region))

#+END_SRC

** hangul

#+begin_src emacs-lisp

(set-fontset-font t 'hangul (font-spec :name "NanumGothicCoding"))

#+end_src

** wgrep

#+BEGIN_SRC emacs-lisp
  (use-package wgrep
    :config
    ;;버퍼를 자동으로 저장
    (setq wgrep-auto-save-buffer t)
    ;;키 바인딩?
    (setq wgrep-enable-key "r")
    ;;읽기전용 버퍼 수정
    (setq wgrep-change-readonly-file t))
#+END_SRC


** clean-aindent-mode

#+begin_src emacs-lisp

	(defun my-pkg-init()
		(electric-indent-mode nil)  ; no electric indent, auto-indent is sufficient
		(clean-aindent-mode t)
		(setq clean-aindent-is-simple-indent t)
		(define-key global-map (kbd "RET") 'newline-and-indent))

	(use-package clean-aindent-mode
		:config
		(add-hook 'after-init-hook 'my-pkg-init))

	(use-package dtrt-indent
		:disabled
		:config
		(dtrt-indent-mode 1))

#+end_src

** Diff
*** diff-hl
#+BEGIN_SRC emacs-lisp

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;; PACKAGE: diff-hl                             ;;
  ;;                                              ;;
  ;; GROUP: Programming -> Tools -> Vc -> Diff Hl ;;
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  (use-package diff-hl
    :ensure t
    :config
    (global-diff-hl-mode)
    (add-hook 'dired-mode-hook 'diff-hl-dired-mode))

  ;; show important whitespace in diff-mode
  (add-hook 'diff-mode-hook (lambda ()
                              (setq-local whitespace-style
                                          '(face
                                            tabs
                                            tab-mark
                                            spaces
                                            space-mark
                                            trailing
                                            indentation::space
                                            indentation::tab
                                            newline
                                            newline-mark))
                              (whitespace-mode 1)))

#+END_SRC

* Files
#+begin_src emacs-lisp

  (setq large-file-warning-threshold 100000000) ;; size in bytes

  (add-hook 'dired-mode-hook 'auto-revert-mode)

  (setq dired-listing-switches "-lha --group-directories-first")


  (if (not (file-exists-p backup-directory))
      (make-directory backup-directory t))
  (setq
   make-backup-files t        ; backup a file the first time it is saved
   backup-directory-alist `((".*" . ,backup-directory)) ; save backup files in ~/.backups
   backup-by-copying t     ; copy the current file into backup directory
   version-control t   ; version numbers for backup files
   delete-old-versions t   ; delete unnecessary versions
   kept-old-versions 6     ; oldest versions to keep when a new numbered backup is made (default: 2)
   kept-new-versions 9 ; newest versions to keep when a new numbered backup is made (default: 2)
   auto-save-default t ; auto-save every buffer that visits a file
   auto-save-timeout 20 ; number of seconds idle time before auto-save (default: 30)
   auto-save-interval 200 ; number of keystrokes between auto-saves (default: 300)
   )
#+end_src

** Get Current file path

#+BEGIN_SRC emacs-lisp

  (defun buffer-kill-path ()
    "copy buffer's full path to kill ring"
    (interactive)
    (kill-new (buffer-file-name)))

  ;;  (define-key c++-mode-map (kbd "C-c C-f") 'buffer-kill-path)
  (define-key global-map (kbd "C-c C-f") 'buffer-kill-path)

#+END_SRC

* Utility
** flyspell
#+BEGIN_SRC emacs-lisp

  ;; GROUP: Processes -> Flyspell
  (if (executable-find "aspell")
      (progn
        (setq ispell-program-name "aspell")
        ;;(setq ispell-extra-args '("--sug-mode=ultra"))
        )
    (setq ispell-program-name "ispell"))

  ;(add-hook 'text-mode-hook 'flyspell-mode)
  (add-hook 'org-mode-hook 'flyspell-mode)
  ;(add-hook 'prog-mode-hook 'flyspell-prog-mode)

#+END_SRC
* Runtime Performance

Dial the GC threshold back down so that garbage collection happens more frequently but in less time.

#+begin_src emacs-lisp

  ;; Make gc pauses faster by decreasing the threshold.
  (setq gc-cons-threshold (* 2 1000 1000))

#+end_src
