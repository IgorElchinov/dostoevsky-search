 if (cur > 19) {
                for (int l = cur -1; l >= 0; l--) {
                    printf("%d", ((symbol_code >> l) & 1u));
                }
                printf("\n");
            }