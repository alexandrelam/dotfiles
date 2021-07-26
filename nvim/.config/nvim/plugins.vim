"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => vim plug
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

call plug#begin('~/.vim/plugged')

" HTML CSS shortcut
Plug 'mattn/emmet-vim'

" Syntastic pour les vérifications d'erreurs 
Plug 'vim-syntastic/syntastic'

" Fuzzyfind file
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'junegunn/fzf.vim'

" Intellisense
Plug 'neoclide/coc.nvim', {'branch': 'release'}

" Custom bar + tab
Plug 'vim-airline/vim-airline'

" gruvbox
Plug 'morhetz/gruvbox'

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

" Initialize plugin system
call plug#end()
