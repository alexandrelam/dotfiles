"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => General
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Combien de lignes sont enregistrées dans l'historique
set history=500

" Numérote les lignes
set nu

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
syntax enable
set encoding=utf8

let g:gruvbox_contrast_dark = 'hard'
colorscheme gruvbox
set bg=dark

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
set shiftwidth=4
set tabstop=4

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
