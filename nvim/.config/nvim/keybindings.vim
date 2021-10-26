"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => General
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Combien de lignes sont enregistrées dans l'historique
set history=500

" Numérote les lignes
set nu

" no wrap
set nowrap

" mouse support
set mouse=a

" Recharge le fichier si il est modifié de l'extérieur
set autoread
au FocusGained,BufEnter * checktime

" Map de la touche leader
let mapleader = ","

" Sauvegarde rapide
nmap <leader>w :w!<cr>

" Montre la position actuelle du curseur
set ruler

" Un buffer est caché si il est abandonné
set hid

" Configure la touche supprimer correctement
set backspace=eol,start,indent
set whichwrap+=<,>,h,l

" Améliore la recherche
set ignorecase
set smartcase
set hlsearch
set incsearch

" Redessine après avoir exécuté une marcro
set lazyredraw

" Active les regular expression
set magic

" Affiche les couples de parenthèses
set showmatch
set mat=2

" Désactive les sons d'erreur
set noerrorbells
set novisualbell
set t_vb=
set tm=500

" Ajoute une marge de 1 à gauche
set foldcolumn=1



"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Colors and Fonts
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Enable syntax highlighting
syntax on 
set encoding=utf8

colorscheme onedark
" set bg=dark

"Use 24-bit (true-color) mode in Vim/Neovim when outside tmux.
"If you're using tmux version 2.2 or later, you can remove the outermost $TMUX check and use tmux's 24-bit color support
"(see < http://sunaku.github.io/tmux-24bit-color.html#usage > for more information.)
if (empty($TMUX))
  if (has("nvim"))
    "For Neovim 0.1.3 and 0.1.4 < https://github.com/neovim/neovim/pull/2198 >
    let $NVIM_TUI_ENABLE_TRUE_COLOR=1
  endif
  "For Neovim > 0.1.5 and Vim > patch 7.4.1799 < https://github.com/vim/vim/commit/61be73bb0f965a895bfb064ea3e55476ac175162 >
  "Based on Vim patch 7.4.1770 (`guicolors` option) < https://github.com/vim/vim/commit/8a633e3427b47286869aa4b96f2bfc1fe65b25cd >
  " < https://github.com/neovim/neovim/wiki/Following-HEAD#20160511 >
  if (has("termguicolors"))
    set termguicolors
  endif
endif


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Fichiers, backups and undo
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

set nobackup
set nowb
set noswapfile



"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Text, tab and indent related
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Utiliser des espaces à la place des tabs
set expandtab

set smarttab

" 1 tab=4 espaces
set shiftwidth=2
set softtabstop=2
set tabstop=2

set lbr
set tw=500

set ai 
set si 


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Déplacements
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Déplacements entre les fenêtres
map <C-j> <C-W>j
map <C-k> <C-W>k
map <C-h> <C-W>h
map <C-l> <C-W>l

" Déplacements entre les buffers 
map gk :bn<cr>
map gj :bp<cr>
map gx :bd<cr>  

nnoremap ` ù
nnoremap ù `
