import streamlit as st
import pandas as pd


def main():
    uploaded_file = st.file_uploader("Загрузите файл с данными", type=["txt", "csv"])

    if uploaded_file is not None:
        try:
            df = pd.read_csv(uploaded_file, header=None, delimiter=' ')
            arr_sizes = df.iloc[0].dropna().values
            merge_random = df.iloc[1].dropna().values
            merge_reverse_sorted = df.iloc[2].dropna().values
            merge_almost_sorted = df.iloc[3].dropna().values
            combine_random = df.iloc[4].dropna().values
            combine_reverse_sorted = df.iloc[5].dropna().values
            combine_almost_sorted = df.iloc[6].dropna().values

            if len({len(arr_sizes), len(merge_random), len(merge_reverse_sorted)}) != 1:
                st.error("Ошибка: массивы должны быть одинаковой длины.")
                return

            data = pd.DataFrame({
                'Array size': arr_sizes,
                'Merge (Random)': merge_random,
                'Merge (Reversed)': merge_reverse_sorted,
                'Merge (Almost sorted)': merge_almost_sorted,
                'Merge-Insertion (Random)': combine_random,
                'Merge-Insertion (Reversed)': combine_reverse_sorted,
                'Merge-Insertion (Almost sorted)': combine_almost_sorted
            }).set_index('Array size')

            st.line_chart(data, x_label='Array size', y_label='Execution time (ms)', color=["#E63946", "#F4A261", "#2A9D8F", "#A663CC", "#FFD166", "#4CC9F0"]

)

        except Exception as e:
            st.error(f"Ошибка при обработке файла: {e}")


if __name__ == "__main__":
    main()
