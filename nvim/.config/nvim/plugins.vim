"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => vim plug
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

call plug#begin('~/.vim/plugged')
" Coloschemes
Plug 'morhetz/gruvbox'
Plug 'joshdick/onedark.vim'

" HTML CSS shortcut
Plug 'mattn/emmet-vim'

" Syntastic pour les vérifications d'erreurs 
Plug 'vim-syntastic/syntastic'

" Telescope fuzzyfind
Plug 'nvim-lua/plenary.nvim'
Plug 'nvim-telescope/telescope.nvim'

" Intellisense
Plug 'neoclide/coc.nvim', {'branch': 'release'}

" Custom bar + tab
Plug 'vim-airline/vim-airline'

" indent guides
Plug 'Yggdroot/indentLine'

" git fugitive
Plug 'tpope/vim-fugitive'

" markdown preview
Plug 'iamcco/markdown-preview.nvim', { 'do': { -> mkdp#util#install() }, 'for': ['markdown', 'vim-plug']}

" vim vue 
Plug 'posva/vim-vue'

" html pug
Plug 'digitaltoad/vim-pug'

" Better syntax highlighting
Plug 'sheerun/vim-polyglot'

" Initialize plugin system
call plug#end()
