(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(ansi-color-faces-vector
   [default default default italic underline success warning error])
 '(column-number-mode t)
 '(custom-enabled-themes (quote (tango-dark)))
 '(indent-tabs-mode nil)
 '(show-paren-mode t)
 '(tool-bar-mode nil))

;; C++ settings
(load "/usr/share/emacs/site-lisp/clang-format-5.0/clang-format.el")
(global-set-key [C-M-tab] 'clang-format-region)

(add-hook 'c-mode-common-hook
          (function (lambda () (local-set-key (kbd "TAB")
                                              'clang-format-region))))
(add-hook 'c++-mode-hook
          (function (lambda () (local-set-key (kbd "TAB")
                                              'clang-format-region))))